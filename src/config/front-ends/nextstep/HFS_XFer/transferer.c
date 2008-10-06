#include "rsys/common.h"
#include "xfer.h"

LONGINT sizetocopy, sizecopied, cantcopydir;
INTEGER BreakCopy;
DialogPtr piechartdp;

LONGINT ischild(LONGINT dir1, LONGINT dir2)
{
    CInfoPBRec pb;
    OSErr err;
    
    if (dir1 == dir2)
        return -1;
    pb.dirInfo.ioCompletion = 0;
    pb.dirInfo.ioNamePtr = 0;
    pb.dirInfo.ioVRefNum = -SFSaveDisk;
    pb.dirInfo.ioFDirIndex = -1;
    pb.dirInfo.ioDrDirID = dir1;
    while (pb.dirInfo.ioDrDirID > 1) {
        err = xPBGetCatInfo(&pb, FALSE);
	if (err != noErr)
	    doerror(err, "\pPBGetCatInfo");
        if (pb.dirInfo.ioDrParID == dir2)
/*-->*/     return pb.dirInfo.ioDrDirID;
        pb.dirInfo.ioDrDirID = pb.dirInfo.ioDrParID;
    }
    return 0;
}

INTEGER ask(char * s1, Str255 s2)
{
    INTEGER retval;
    
    ParamText((StringPtr) s1, s2, (StringPtr) 0, (StringPtr) 0);
    retval = CautionAlert(ASKALERT, (ProcPtr) 0);
    if (sizetocopy > 0)
        DrawDialog(piechartdp);
    return retval;
}

void makepiechart( void )
{
    if (sizetocopy > 0) {
        piechartdp = GetNewDialog(PIECHARTID, (Ptr) 0, (WindowPtr) -1);
        SetPort(piechartdp);
        DrawDialog(piechartdp);
    } else
        piechartdp = 0;
}

void updatepiechart( void )
{
    Rect r;
    Handle h;
    INTEGER itype;
    GrafPtr saveport;
    
    if (piechartdp) {
        GetPort(&saveport);
        SetPort(piechartdp);
        GetDItem(piechartdp, PIECHARTITEM, &itype, &h, &r);
        FrameOval(&r);
        FillArc(&r, 0, (long) sizecopied * 360 / sizetocopy, black);
        SetPort(saveport);
    }
}

#define PREALLOCATE /* */

OSErr CopyFork(forktype fork, StringPtr name, LONGINT fromid,
			LONGINT toid, INTEGER srcvrn, INTEGER dstvrn)
{
    HParamBlockRec rdio, wrio;
    OSErr err;
    EventRecord event;
#if !defined (UNIX)
#define BUFSIZE	(1L * 1024)
#else
#define BUFSIZE	(8L * 1024)
#endif
    char buf[BUFSIZE];
    
    rdio.ioParam.ioVRefNum = srcvrn;
    rdio.ioParam.ioNamePtr = name;
    rdio.fileParam.ioDirID = fromid;
#if defined(PREALLOCATE)
    rdio.fileParam.ioFDirIndex = 0;
    err = xPBHGetFInfo((ParmBlkPtr) &rdio, FALSE);
    if (err != noErr) {
	doerror(err, "\pPBGetFInfo");
/*-->*/ return err;
    }
    rdio.fileParam.ioDirID = fromid;
#endif /* PREALLOCATE */
    rdio.ioParam.ioPermssn = fsRdPerm;
    rdio.ioParam.ioMisc = 0;
    
    wrio.ioParam.ioVRefNum = dstvrn;
    wrio.ioParam.ioNamePtr = name;
    wrio.ioParam.ioPermssn = fsWrPerm;
    wrio.ioParam.ioMisc = 0;
    wrio.fileParam.ioDirID = toid;
    
    switch (fork) {
    case datafork:
        err = xPBHOpen((HFileParam *)&rdio, FALSE);
        if (err != noErr)
            return err;
        err = xPBHOpen((HFileParam *)&wrio, FALSE);
        if (err != noErr) {
            xPBClose((ioParam *) &rdio, FALSE);
            return err;
        }
#if defined(PREALLOCATE)
	wrio.ioParam.ioReqCount =   rdio.fileParam.ioFlLgLen;
	wrio.ioParam.ioMisc =       (LONGORPTR) rdio.fileParam.ioFlLgLen;
#endif /* PREALLOCATE */
        break;
    case resourcefork:
        err = xPBHOpenRF((HFileParam *)&rdio, FALSE);
        if (err != noErr)
            return err;
        err = xPBHOpenRF((HFileParam *)&wrio, FALSE);
        if (err != noErr) {
            xPBClose((ioParam *) &rdio, FALSE);
            return err;
        }
#if defined(PREALLOCATE)
	wrio.ioParam.ioReqCount =   rdio.fileParam.ioFlRLgLen;
	wrio.ioParam.ioMisc =       (LONGORPTR) rdio.fileParam.ioFlRLgLen;
#endif /* PREALLOCATE */
        break;
    default:
/*-->*/ return fsDSIntErr;
    }
    
#if defined(PREALLOCATE)
    err = xPBAllocContig((ParmBlkPtr) &wrio, FALSE);
    if (err != noErr) {
	if (err != dskFulErr) {
	    err = xPBAllocate((ParmBlkPtr) &wrio, FALSE);
	    if (err != noErr) {
		doerror(err, "\pPBAllocate");
/*-->*/         goto DONE;
	    }
	} else {
	    doerror(err, "\pPBAllocContig");
/*-->*/     goto DONE;
	}
    }
    err = xPBSetEOF((ParmBlkPtr) &wrio, FALSE);
    if (err != noErr) {
	doerror(err, "\pPBSetEOF");
/*-->*/ goto DONE;
    }
#endif /* PREALLOCATE */

    do {
        rdio.ioParam.ioBuffer = (Ptr) buf;
        rdio.ioParam.ioReqCount = BUFSIZE;
        rdio.ioParam.ioPosMode = fsFromMark;
        rdio.ioParam.ioPosOffset = 0;
#if 0
        err = xPBRead((ParmBlkPtr) &rdio, FALSE);
#else /* 0 */
        err = xPBRead((ioParam *) &rdio, FALSE);
#endif /* 0 */
        
        if (err == noErr || (err == eofErr && rdio.ioParam.ioActCount > 0)) {
            wrio.ioParam.ioBuffer = (Ptr) buf;
            wrio.ioParam.ioReqCount = rdio.ioParam.ioActCount;
            wrio.ioParam.ioPosMode = fsFromMark;
            wrio.ioParam.ioPosOffset = 0;
            err = xPBWrite((ioParam *) &wrio, FALSE);
        }
        sizecopied++;
	updatepiechart();
        while (GetNextEvent(keyDownMask, &event))
            if ((event.modifiers & cmdKey) && 
				((event.message & charCodeMask) == '.')) {
		err = xPBClose((ioParam *) &wrio, FALSE);
		if (err != noErr)
		    doerror(err, "\pPBClose");
		wrio.fileParam.ioDirID = toid;
		err = xPBHDelete((HFileParam *) &wrio, FALSE);
		if (err != noErr)
		    doerror(err, "\pPBHDelete");
		BreakCopy = TRUE;
		return eofErr;
	    }
    } while (err == noErr);
    
    sizecopied--;
DONE:
    xPBClose((ioParam *) &rdio, FALSE);
    xPBClose((ioParam *) &wrio, FALSE);
    return err;
}

void warnaboutincest( void )
{
    ParamText((StringPtr) "\pDirectories may not be transfered to their offspring.",
    								0, 0, 0);
    StopAlert(ONEPARAMALERT, (ProcPtr) 0);
}

INTEGER copy1file(INTEGER srcvrn, INTEGER dstvrn, LONGINT srcdirid,
				LONGINT dstdirid, Str255 s, BOOLEAN doit)
{
    HParamBlockRec hpb;
    OSErr err;
    INTEGER retval, type, filesize;
    Handle h;
    Rect r;
    INTEGER savevdd, savevfd;
    Str255 s2;
    
    if (BreakCopy)
        return FALSE;
    retval = TRUE;
    hpb.fileParam.ioVRefNum = srcvrn;
    hpb.fileParam.ioDirID = srcdirid;
    hpb.fileParam.ioFDirIndex = 0;
    hpb.fileParam.ioNamePtr = s;
    err = xPBGetCatInfo((CInfoPBPtr) &hpb, FALSE);
    if (err != noErr) {
	doerror(err, "\pPBGetCatInfo");
/*-->*/ return FALSE;
    }
    if (hpb.fileParam.ioFlFndrInfo.fdFlags & fInvisible)
/*-->*/ return FALSE;
    if (cantcopydir == hpb.fileParam.ioDirID) {
        if (!doit)
            warnaboutincest();
/*-->*/ return FALSE;
    }
    if (hpb.fileParam.ioFlAttrib & ISDIRMASK) {
	srcdirid = hpb.fileParam.ioDirID;
	if (doit) {
	    hpb.fileParam.ioVRefNum = dstvrn;
	    hpb.fileParam.ioDirID = dstdirid;
            err = xPBDirCreate((HFileParam *)&hpb, FALSE);
            if (err == dupFNErr) {
	        if (!verifydiroverwrite ||
	    			  ask("\poverwrite directory", s) == OK) {
		    savevdd = verifydirdelete;
		    savevfd = verifyfiledelete;
		    verifyfiledelete = 0;
		    verifydirdelete = 0;
	            delete1file(dstvrn, dstdirid, s);
		    verifyfiledelete = savevfd;
		    verifydirdelete = savevdd;
		    err = xPBDirCreate((HFileParam *)&hpb, FALSE);
		    if (err != noErr) {
	                doerror(err, "\pPBDirCreate");
/*-->*/                 return FALSE;
		    }
	        } else
/*-->*/             return FALSE;
            } else if (err != noErr) {
	        doerror(err, "\pPBDirCreate");
/*-->*/         return FALSE;
	    }
	    dstdirid = hpb.fileParam.ioDirID;
        }
	hpb.fileParam.ioVRefNum = srcvrn;
	hpb.fileParam.ioNamePtr = s2;
        for (hpb.fileParam.ioFDirIndex = 1; err == noErr;
 					   hpb.fileParam.ioFDirIndex++) {
 	    hpb.fileParam.ioDirID = srcdirid;
	    err = xPBGetCatInfo((CInfoPBPtr)&hpb, FALSE);
	    if (err == noErr)
		retval &= copy1file(srcvrn, dstvrn, srcdirid, dstdirid,
								s2, doit);
	}
    } else {
        filesize =  (hpb.fileParam.ioFlLgLen + BUFSIZE - 1) / BUFSIZE
		      + (hpb.fileParam.ioFlRLgLen + BUFSIZE - 1) / BUFSIZE;
        if (!doit) {
	    sizetocopy += filesize;
	    return retval;
        }
	hpb.fileParam.ioVRefNum = dstvrn;
	hpb.fileParam.ioDirID = dstdirid;
	err = xPBHCreate((HFileParam *)&hpb, FALSE);
	if (err == dupFNErr) {
	    if (!verifyfileoverwrite || ask("\poverwrite file", s) == OK) {
		savevfd = verifyfiledelete;
		verifyfiledelete = 0;
	        delete1file(dstvrn, dstdirid, s);
		verifyfiledelete = savevfd;
		err = xPBHCreate((HFileParam *)&hpb, FALSE);
		if (err != noErr) {
	            doerror(err, "\pPBHCreate");
/*-->*/             return FALSE;
		}
	    } else {
	        sizecopied += (hpb.fileParam.ioFlLgLen + BUFSIZE - 1) 
	        / BUFSIZE + 
	        (hpb.fileParam.ioFlRLgLen + BUFSIZE - 1) / BUFSIZE;
/*-->*/         return FALSE;
	    }
	} else if (err != noErr) {
	    doerror(err, "\pPBHCreate");
/*-->*/     return FALSE;
	}
	if (piechartdp) {
	    GetDItem(piechartdp, FILENAMEITEM, &type, &h, &r);
	    SetIText(h, s);
	}
	err = CopyFork(datafork, s, srcdirid, hpb.fileParam.ioDirID,
    						           srcvrn, dstvrn);
	if (err != eofErr) {
	    doerror(err, "\pCopyFork");
	    return FALSE;
	}
	err = CopyFork(resourcefork, s, srcdirid, hpb.fileParam.ioDirID,
    						           srcvrn, dstvrn);
	if (err != eofErr) {
	    doerror(err, "\pCopyFork");
	    return FALSE;
	}
	err = xPBSetCatInfo((CInfoPBPtr)&hpb, FALSE);
	if (err != noErr) {
	    doerror(err, "\pPBSetCatInfo");
/*-->*/     return FALSE;
	}
    }
    return retval;
}

INTEGER move1file(INTEGER srcvrn, INTEGER dstvrn, LONGINT srcdirid,
				LONGINT dstdirid, Str255 s, BOOLEAN doit)
{
    CMovePBRec cpb;
    CInfoPBRec hpb;
    OSErr err;
    INTEGER savevdd, savevfd, retval, vrn;
    ParamBlockRec pb;
    
    if (BreakCopy)
        return FALSE;
    retval = TRUE;
    if (dstvrn == srcvrn) {
        if (!doit)
/*-->*/     return FALSE;
        cpb.ioCompletion = 0;
        cpb.ioNamePtr = s;
        cpb.ioVRefNum = srcvrn;
        cpb.ioNewName = 0;
        cpb.ioNewDirID = dstdirid;
        cpb.ioDirID = srcdirid;
        err = xPBCatMove(&cpb, FALSE);
        if (err == badMovErr)
            warnaboutincest();
        else if (err == dupFNErr) {
	    hpb.hFileInfo.ioCompletion = 0;
	    hpb.hFileInfo.ioNamePtr = s;
	    hpb.hFileInfo.ioVRefNum = srcvrn;
	    hpb.hFileInfo.ioFDirIndex = 0;
	    hpb.hFileInfo.ioDirID = srcdirid;
	    err = xPBGetCatInfo(&hpb, FALSE);
	    if (err != noErr) {
	        doerror(err, "\pPBGetCatInfo");
/*-->*/         return FALSE;
	    }
	    if (hpb.hFileInfo.ioFlAttrib & ISDIRMASK) {
	        if (!verifydiroverwrite ||
	        		  ask("\poverwrite directory", s) == OK) {
		    savevdd = verifydirdelete;
		    savevfd = verifyfiledelete;
		    verifyfiledelete = 0;
		    verifydirdelete = 0;
	            delete1file(dstvrn, dstdirid, s);
		    verifyfiledelete = savevfd;
		    verifydirdelete = savevdd;
                    err = xPBCatMove(&cpb, FALSE);
	            if (err != noErr)
	                doerror(err, "\pPBCatMove");
		}
	    } else {
	        if (!verifyfileoverwrite || 
	        		      ask("\poverwrite file", s) == OK) {
		    savevfd = verifyfiledelete;
		    verifyfiledelete = 0;
	            delete1file(dstvrn, dstdirid, s);
		    verifyfiledelete = savevfd;
                    err = xPBCatMove(&cpb, FALSE);
	            if (err != noErr)
	                doerror(err, "\pPBCatMove");
		}
	    }
	} else if (err != noErr)
	    doerror(err, "\pPBCatMove");
    } else {
        if (hpb.hFileInfo.ioFlFndrInfo.fdFlags & fInvisible)
/*-->*/     return FALSE;
	pb.volumeParam.ioVolIndex = 0;
	pb.volumeParam.ioVRefNum = vrn;
	pb.volumeParam.ioNamePtr = 0;
	err = xPBGetVInfo((volumeParam *) &pb, FALSE);
	if (err != noErr) {
	    doerror(err, "\pPBGetVInfo");
/*-->*/     return FALSE;
	}
	if (pb.volumeParam.ioVAtrb & VOLLOCKEDMASK) {
	    if (doit) {
		ParamText((StringPtr)
		     "\pFiles can not be moved from a locked disk." , 0, 0, 0);
		StopAlert(ONEPARAMALERT, (ProcPtr) 0);
	    }
/*-->*/     return FALSE;
	}

        if ((retval = copy1file(srcvrn, dstvrn, srcdirid, dstdirid, s, doit))
								   && doit) {
	    savevdd = verifydirdelete;
	    savevfd = verifyfiledelete;
	    verifyfiledelete = 0;
	    verifydirdelete = 0;
            delete1file(srcvrn, srcdirid, s);
	    verifyfiledelete = savevfd;
	    verifydirdelete = savevdd;
	}
    }
    return retval;
}

void printsillywarning( void )
{
    ParamText((StringPtr) "\pCopying files onto themselves does nothing",
								    0, 0, 0);
    NoteAlert(ONEPARAMALERT, (ProcPtr) 0);
}

void noroom(INTEGER needed, INTEGER avail)
{
    Str255 s1, s2;
    
    NumToString((LONGINT) needed, s1);
    NumToString((LONGINT) avail, s2);
    ParamText(s1, s2, 0, 0);
    StopAlert(NOROOMALERTID, (ProcPtr) 0);
}

void getnameandfromdirid(Str255 *sp, LONGINT *fromdirid)
{
    CInfoPBRec cpb;

    if (globalreply.fName[0] == 0) {
        cpb.hFileInfo.ioFDirIndex = -1;
        cpb.hFileInfo.ioDirID = globalreply.fType;
        cpb.hFileInfo.ioVRefNum = -SFSaveDisk;
        cpb.hFileInfo.ioNamePtr = *sp;
        PBGetCatInfo(&cpb, FALSE);
	if (sp[0] != 0)
	    *fromdirid = cpb.hFileInfo.ioFlParID;
	else
            *fromdirid = CurDirStore;
    } else {
        *fromdirid = CurDirStore;
	bcopy(globalreply.fName, *sp, globalreply.fName[0] + 1);
    }
}

void dotransfer(INTEGER (*fp)(INTEGER, INTEGER, LONGINT,
						LONGINT, Str255, BOOLEAN))
{
    LONGINT fromdirid;
    Str255 sp;
    OSErr err;
    ParamBlockRec pb;
    GrafPtr saveport;
    
    getnameandfromdirid(&sp, &fromdirid);
    if (SFSaveDisk == destdisk)
	cantcopydir = ischild(destdir, fromdirid);
    else
        cantcopydir = 0;
    if (cantcopydir == -1) {
        printsillywarning();
/*-->*/ return;
    }
    sizetocopy = 0;
    sizecopied = 0;
    BreakCopy = FALSE;    
    (*fp)(-SFSaveDisk, -destdisk, fromdirid, destdir, sp, FALSE);
    GetPort(&saveport);
    
    pb.volumeParam.ioCompletion = 0;
    pb.volumeParam.ioNamePtr = 0;
    pb.volumeParam.ioVRefNum = -destdisk;
    pb.volumeParam.ioVolIndex = 0;
    err = xPBGetVInfo((volumeParam *)&pb, FALSE);
    if (err != noErr) {
	doerror(err, "\pPBGetVInfo");
	return;
    }
    if (sizetocopy > pb.volumeParam.ioVFrBlk *
					pb.volumeParam.ioVAlBlkSiz / BUFSIZE) {
        noroom(sizetocopy * BUFSIZE / pb.volumeParam.ioVAlBlkSiz,
						    pb.volumeParam.ioVFrBlk);
        return;
    }

    makepiechart();
    updatepiechart();
    (*fp)(-SFSaveDisk, -destdisk, fromdirid, destdir, sp, TRUE);
    updatepiechart();
    SetPort(saveport);
    if (piechartdp)
        DisposDialog(piechartdp);
    sizetocopy = 0;
    piechartdp = 0;
}

INTEGER docopydisk( DialogPtr dp )
{
    GrafPtr saveport;
    Str255 s;
    ParamBlockRec pb;
    OSErr err;
    
    if (!caneject(dp)) {
	ParamText((StringPtr) "\pOnly floppies may be copied with this option.", 0, 0, 0);
	StopAlert(ONEPARAMALERT, (ProcPtr) 0);
	return 0;
    }
    if (SFSaveDisk == destdisk) {
	warnaboutincest();
	return 0;
    }
    sizetocopy = 0;
    sizecopied = 0;
    piechartdp = (GrafPtr) 0;
    GetPort(&saveport);
    BreakCopy = FALSE;
    pb.volumeParam.ioCompletion = 0;
    pb.volumeParam.ioNamePtr = s;
    pb.volumeParam.ioVRefNum = -SFSaveDisk;
    pb.volumeParam.ioVolIndex = 0;
    err = xPBGetVInfo((volumeParam *)&pb, FALSE);
    if (err != noErr) {
	doerror(err, "\pPBGetVInfo");
	return 0;
    }
    copy1file(-SFSaveDisk, -destdisk, 1, destdir, s, FALSE);
    pb.volumeParam.ioCompletion = 0;
    pb.volumeParam.ioNamePtr = 0;
    pb.volumeParam.ioVRefNum = -destdisk;
    pb.volumeParam.ioVolIndex = 0;
    err = xPBGetVInfo((volumeParam *)&pb, FALSE);
    if (err != noErr) {
	doerror(err, "\pPBGetVInfo");
	return 0;
    }
    if (sizetocopy > pb.volumeParam.ioVFrBlk *
					pb.volumeParam.ioVAlBlkSiz / BUFSIZE) {
        noroom(sizetocopy * BUFSIZE / pb.volumeParam.ioVAlBlkSiz,
						    pb.volumeParam.ioVFrBlk);
        return 0;
    }

    makepiechart();
    updatepiechart();
    copy1file(-SFSaveDisk, -destdisk, 1, destdir, s, TRUE);
    updatepiechart();
    SetPort(saveport);
    if (piechartdp)
        DisposDialog(piechartdp);
    piechartdp = 0;
    sizetocopy = 0;
    return 101;
}
