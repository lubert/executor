#include "localvolume.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

using namespace Executor;

#if 1
PlainDataFork::PlainDataFork(fs::path path)
{
    fd = open(path.string().c_str(), O_RDWR | O_CREAT, 0644);
//    std::cout << "ACCESSING FILE: " << path << std::endl;
}
PlainDataFork::~PlainDataFork()
{
    close(fd);
}

size_t PlainDataFork::getEOF()
{
    return lseek(fd, 0,SEEK_END);
}
void PlainDataFork::setEOF(size_t sz)
{
    ftruncate(fd, sz);
}
size_t PlainDataFork::read(size_t offset, void *p, size_t n)
{
    lseek(fd, offset, SEEK_SET);
    ssize_t done;
    
    do
    {
        done = ::read(fd, p, n);
    } while(errno == EINTR);
    
    return done;
}
size_t PlainDataFork::write(size_t offset, void *p, size_t n)
{
    lseek(fd, offset, SEEK_SET);
    ssize_t done;
    
    do
    {
        done = ::write(fd, p, n);
    } while(errno == EINTR);
    
    if(done != n)
    {
        std::cout << "short write: " << done << " of " << n << " ; errno = " << errno << std::endl;
    }
    return done;
}
#else
PlainDataFork::PlainDataFork(fs::path path)
    : stream(path)
{
    std::cout << "ACCESSING FILE: " << path << std::endl;
}
PlainDataFork::~PlainDataFork()
{
}

size_t PlainDataFork::getEOF()
{
    stream.seekg(0, std::ios::end);
    return stream.tellg();
}
void PlainDataFork::setEOF(size_t sz)
{
    
}
size_t PlainDataFork::read(size_t offset, void *p, size_t n)
{
    stream.seekg(offset);
    stream.read((char*)p, n);
    stream.clear(); // ?
    return stream.gcount();
}
size_t PlainDataFork::write(size_t offset, void *p, size_t n)
{
    stream.seekp(offset);
    stream.write((char*)p, n);
    return n;
}
#endif