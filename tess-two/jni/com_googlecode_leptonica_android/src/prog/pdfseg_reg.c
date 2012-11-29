/*====================================================================*
 -  Copyright (C) 2001 Leptonica.  All rights reserved.
 -  This software is distributed in the hope that it will be
 -  useful, but with NO WARRANTY OF ANY KIND.
 -  No author or distributor accepts responsibility to anyone for the
 -  consequences of using this software, or for whether it serves any
 -  particular purpose or works at all, unless he or she says so in
 -  writing.  Everyone is granted permission to copy, modify and
 -  redistribute this source code, for commercial or non-commercial
 -  purposes, with the following restrictions: (1) the origin of this
 -  source code must not be misrepresented; (2) modified versions must
 -  be plainly marked as such; and (3) this notice may not be removed
 -  or altered from any source or modified source distribution.
 *====================================================================*/

/*
 * pdfseg_reg.c
 *
 *   Generates segmented images and encodes them efficiently in pdf.
 *   The encoding is mixed-raster, with the image parts encoded as
 *   DCT at one resolution and the non-image parts encoded at (typically)
 *   a higher resolution.
 *
 *   Uses 6 images, all segmented and scaled to a fixed width
 */

#include "allheaders.h"

    /* All images scaled to this width  */
static const l_int32  WIDTH = 800;

main(int    argc,
     char **argv)
{
l_int32      h;
l_float32    scalefactor;
BOX         *box;
BOXA        *boxa1, *boxa2;
BOXAA       *baa;
PIX         *pix1, *pix2, *pix3, *pix4, *pix5, *pix6, *pix7, *pix8, *pix9;
L_REGPARAMS  *rp;

    if (regTestSetup(argc, argv, &rp))
        return 1;

    lept_rmdir("segtest");
    lept_mkdir("segtest");
    baa = boxaaCreate(5);

        /* Image region input.  */
    pix1 = pixRead("wet-day.jpg");
    pix2 = pixScaleToSize(pix1, WIDTH, 0);
    pixWrite("/tmp/segtest/0.jpg", pix2, IFF_JFIF_JPEG);
    regTestCheckFile(rp, "/tmp/segtest/0.jpg");   /* 0 */
    box = boxCreate(105, 161, 620, 872);   /* image region */
    boxa1 = boxaCreate(1);
    boxaAddBox(boxa1, box, L_INSERT);
    boxaaAddBoxa(baa, boxa1, L_INSERT);
    pixDestroy(&pix1);
    pixDestroy(&pix2);

        /* Compute image region at w = 2 * WIDTH */
    pix1 = pixRead("candelabrum-11.jpg");
    pix2 = pixScaleToSize(pix1, WIDTH, 0);
    pix3 = pixConvertTo1(pix2, 100);
    pix4 = pixExpandBinaryPower2(pix3, 2);  /* w = 2 * WIDTH */
    pix5 = pixGenHalftoneMask(pix4, NULL, NULL, 1);
    pix6 = pixMorphSequence(pix5, "c20.1 + c1.20", 0);
    pix7 = pixMaskConnComp(pix6, 8, &boxa1);
    pix8 = pixReduceBinary2(pix7, NULL);  /* back to w = WIDTH */
    pix9 = pixBackgroundNormSimple(pix2, pix8, NULL);
    pixWrite("/tmp/segtest/1.jpg", pix9, IFF_JFIF_JPEG);
    regTestCheckFile(rp, "/tmp/segtest/1.jpg");   /* 1 */
    boxa2 = boxaTransform(boxa1, 0, 0, 0.5, 0.5);  /* back to w = WIDTH */
    boxaaAddBoxa(baa, boxa2, L_INSERT);
    pixDestroy(&pix1);
    pixDestroy(&pix2);
    pixDestroy(&pix3);
    pixDestroy(&pix4);
    pixDestroy(&pix5);
    pixDestroy(&pix6);
    pixDestroy(&pix7);
    pixDestroy(&pix8);
    pixDestroy(&pix9);
    boxaDestroy(&boxa1);

        /* Use mask to find image region */
    pix1 = pixRead("lion-page.00016.jpg");
    pix2 = pixScaleToSize(pix1, WIDTH, 0);
    pixWrite("/tmp/segtest/2.jpg", pix2, IFF_JFIF_JPEG);
    regTestCheckFile(rp, "/tmp/segtest/2.jpg");   /* 2 */
    pix3 = pixRead("lion-mask.00016.tif");
    pix4 = pixScaleToSize(pix3, WIDTH, 0);
    boxa1 = pixConnComp(pix4, NULL, 8);
    boxaaAddBoxa(baa, boxa1, L_INSERT);
    pixDestroy(&pix1);
    pixDestroy(&pix2);
    pixDestroy(&pix3);
    pixDestroy(&pix4);

        /* Compute image region at full res */
    pix1 = pixRead("rabi.png");
    scalefactor = (l_float32)WIDTH / (l_float32)pixGetWidth(pix1);
    pix2 = pixScaleToGray(pix1, scalefactor);
    pixWrite("/tmp/segtest/3.jpg", pix2, IFF_JFIF_JPEG);
    regTestCheckFile(rp, "/tmp/segtest/3.jpg");   /* 3 */
    pix3 = pixGenHalftoneMask(pix1, NULL, NULL, 0);
    pix4 = pixMorphSequence(pix3, "c20.1 + c1.20", 0);
    boxa1 = pixConnComp(pix4, NULL, 8);
    boxa2 = boxaTransform(boxa1, 0, 0, scalefactor, scalefactor);
    boxaaAddBoxa(baa, boxa2, L_INSERT);
    pixDestroy(&pix1);
    pixDestroy(&pix2);
    pixDestroy(&pix3);
    pixDestroy(&pix4);
    boxaDestroy(&boxa1);

        /* Page with no image regions */
    pix1 = pixRead("lucasta-47.jpg");
    pix2 = pixScaleToSize(pix1, WIDTH, 0);
    boxa1 = boxaCreate(1);
    pixWrite("/tmp/segtest/4.jpg", pix2, IFF_JFIF_JPEG);
    regTestCheckFile(rp, "/tmp/segtest/4.jpg");   /* 4 */
    boxaaAddBoxa(baa, boxa1, L_INSERT);
    pixDestroy(&pix1);
    pixDestroy(&pix2);

        /* Page that is all image */
    pix1 = pixRead("map1.jpg");
    pix2 = pixScaleToSize(pix1, WIDTH, 0);
    pixWrite("/tmp/segtest/5.jpg", pix2, IFF_JFIF_JPEG);
    regTestCheckFile(rp, "/tmp/segtest/5.jpg");   /* 5 */
    h = pixGetHeight(pix2);
    box = boxCreate(0, 0, WIDTH, h);
    boxa1 = boxaCreate(1);
    boxaAddBox(boxa1, box, L_INSERT);
    boxaaAddBoxa(baa, boxa1, L_INSERT);
    pixDestroy(&pix1);
    pixDestroy(&pix2);

        /* Save the boxaa file */
    boxaaWrite("/tmp/segtest/seg.baa", baa);
    regTestCheckFile(rp, "/tmp/segtest/seg.baa");   /* 6 */

        /* Do the conversion */
    l_pdfSetDateAndVersion(FALSE);
    convertSegmentedFilesToPdf("/tmp/segtest", ".jpg", 100, L_G4_ENCODE,
                               140, baa, 75, 0.6, "Segmentation Test",
                               "/tmp/pdfseg.7.pdf");
    regTestCheckFile(rp, "/tmp/pdfseg.7.pdf");   /* 7 */

    boxaaDestroy(&baa);
    regTestCleanup(rp); 
    return 0;
}
