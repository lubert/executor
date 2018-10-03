#include "item.h"
#include "itemcache.h"

#include <OSUtil.h>
#include <iostream>

using namespace Executor;

Item::Item(ItemCache& itemcache, CNID parID, CNID cnid, fs::path p, mac_string_view name)
    : itemcache_(itemcache), parID_(parID), cnid_(cnid), path_(std::move(p))
{
    if(name.empty())
        name_ = toMacRomanFilename(path_.filename());
    else
        name_ = name;
}

Item::~Item()
{
    itemcache_.noteItemFreed(cnid_);
}

void Item::deleteItem()
{
    fs::remove(path());
}

void Item::renameItem(mac_string_view newName)
{
    fs::path newPath = path().parent_path() / toUnicodeFilename(newName);
    fs::rename(path(), newPath);
    path_ = std::move(newPath);
    name_ = newName;
}

void Item::moveItem(const fs::path& newParent)
{
    fs::path newPath = newParent / path().filename();
    fs::rename(path(), newPath);
    path_ = std::move(newPath);
}

ItemPtr DirectoryItemFactory::createItemForDirEntry(ItemCache& itemcache, CNID parID, CNID cnid, const fs::directory_entry& e)
{
    if(fs::is_directory(e.path()))
    {
        return std::make_shared<DirectoryItem>(itemcache, parID, cnid, e.path());
    }
    return nullptr;
}

void DirectoryItem::clearCache()
{
    cache_valid_ = false;
    contents_.clear();
    contents_by_name_.clear();
    files_.clear();
}

void DirectoryItem::populateCache(std::vector<ItemPtr> items)
{
    if(cache_valid_)
        return;

    contents_ = std::move(items);

    for(const ItemPtr& item : contents_)
    {
        mac_string nameUpr = item->name();
        ROMlib_UprString(nameUpr.data(), false, nameUpr.size());

        auto [it, inserted] = contents_by_name_.emplace(nameUpr, item);
        if(inserted)
        {
            if(!dynamic_cast<DirectoryItem*>(item.get()))
                files_.push_back(item);
        }
        else
        {
            std::cerr << "duplicate name mapping: " << item->path() << std::endl; 
            std::cerr << "  and: " << it->second->path() << std::endl;
            std::cerr << "  mapped to: " << toUnicodeFilename(nameUpr) << std::endl;
        }
    }

    cache_valid_ = true;
}

ItemPtr DirectoryItem::tryResolve(mac_string_view name)
{
    assert(cache_valid_);
    mac_string nameUpr { name };
    ROMlib_UprString(nameUpr.data(), false, nameUpr.size());
    auto it = contents_by_name_.find(nameUpr);
    if(it != contents_by_name_.end())
        return it->second;
    return {};
}

ItemPtr DirectoryItem::resolve(int index, bool includeDirectories)
{
    assert(cache_valid_);
    const auto& array = includeDirectories ? contents_ : files_;
    if(index >= 1 && index <= array.size())
        return array[index-1];
    throw OSErrorException(fnfErr);
}


void DirectoryItem::deleteItem()
{
    boost::system::error_code ec;
    fs::remove(path() / ".rsrc", ec);
    fs::remove(path() / ".finf", ec);       // TODO: individual ItemFactories should provide this info

    fs::remove(path(), ec);

    if(ec)
    {
        if(ec == boost::system::errc::directory_not_empty)
            throw OSErrorException(fBsyErr);
        else
            throw OSErrorException(paramErr);
    }
}
