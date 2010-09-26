/*
   =============================================================================
	egvdi.c -- a C library test set.
	Version 2 -- 1989-01-24 -- Metacomco / Lattice / D.N. Lynx Crowe

	Watch out for data sizes.

	All pointers in arg lists of GEM functions must be 16 bit WORD's.

	BYTE, WORD, LONG etc are defined in the portab.h include file.
   =============================================================================
*/

/* load fonts not implemented in current ST GEM */

#define	HASFNTLD	0	/* non-zero if vst_load_fonts() is implemented */

#include "stdio.h"
#include "gemlib.h"	/* assorted useful GEM macro's    */
#include "portab.h"	/* include type def macro's etc   */

#define	MYLINE	0x5ABC	/* a user defined line style mask */

/* GLOBALS needed below */

WORD	phys_handle;
WORD	handle;
WORD	dummy;
WORD	pxyarray[4];

setpxy(x1, y1, x2, y2)
WORD x1, y1, x2, y2;
{
	pxyarray[0] = x1;	/* set global points array   */
	pxyarray[1] = y1;	/* from data points supplied */
	pxyarray[2] = x2;
	pxyarray[3] = y2;
}

line(x1, y1, x2, y2)
WORD x1, y1, x2, y2;
{
	pxyarray[0] = x1;
	pxyarray[1] = y1;
	pxyarray[2] = x2;
	pxyarray[3] = y2;		/* set-up array for v_pline() */

	v_pline(handle, 2, pxyarray);	/* call polyline to draw line */

	/* that was ( handle, npoints, array_of_points ) */
}

/* 
*/

line_test()
{
	int   j;	/* loop counter*/

	vsl_udsty(handle, MYLINE);	/* define user def. line  (No 7)   */

	/* polymarkers are point marker shapes for graphs etc */

	vsm_type(handle, 4);		/* use square polymarker (1 to 6)  */
	vsm_height(handle, 10);		/* and make it nice and big. */
	vsm_color(handle, BLACK);	/* set marker color */

	for (j = 1; j < 8; j++) {	/* draw lines with diff attributes */

		vsl_type(handle, j);	/* try each line style */
		vsl_color(handle, j);	/* and each available color */

		/* You can set line termination style too */
		/* N.B: there are only 3 end types */

		vsl_ends(handle, (j-1) & 3, (j-1) & 3);
		line(10, j*10+10, 100, j*10+10);	/* draw a line */
		v_pmarker(handle, 2, pxyarray);		/* mark ends   */

		/* v_pmarker has same syntax as v_pline */
	}

	vsl_color(handle, BLACK);		/* reset */
	vsl_type(handle, SOLID);		/* and line type */
	vsl_ends(handle, ARROWED, ARROWED);	/* set end type for thickness loop */

	for(j = 1; j < 10; j += 2) {		/* i.e. for 1,3,5,7,9   */

		/* you can select line thickness too */

		vsl_width(handle, j);			/* set thickness        */
		line(105, j*10+10, 195, j*10+10);	/* draw a line */
	}

	vsl_ends(handle, SQUARED, SQUARED);	/* reset end types */

	vsl_width(handle, 1);	/* reset the width in case it's used again */
}

/* 
*/

circ_test()
{
	int   radius,k;

	/* vsf_ functions select fill attributes */

	vsf_interior(handle, PATTERN);		/* tell gem to pattern fill */
	vsf_color(handle, BLACK);		/* in fill color black   */

	for(k = 10; k > 0; k--) {

		vsf_style(handle, k);		/* set fill pattern */
		radius = k * 10;
		v_circle(handle, radius+2, 100, radius);
	}
}

box_test()
{
	setpxy(320, 5, 635, 195);
	vsf_color(handle, BLACK);
	vsf_interior(handle, SOLID);
	v_bar(handle, pxyarray);	/* do a rectangle (no border) */

	setpxy(330, 15, 625, 185);
	vsf_interior(handle, PATTERN);
	vsf_style(handle, DOTS);
	vr_recfl(handle, pxyarray);	/* filled rectangle */

	setpxy(340, 25, 615, 175);
	vsf_style(handle, BRICKS);
	v_rfbox(handle, pxyarray);	/* try a rounded & filled box */

	vsf_interior(handle, HATCH);
	vsf_color(handle, 3);
	vsf_style(handle, GRID);
	v_ellipse(handle, 479, 130, 120, 35);	/* put filled ellipse */

	/* that was xpos, ypos, xradius, yradius */

	vsf_color(handle, BLACK);
}

/* 
*/

arc_test()
{
	int radius, revs, da, ang;

	radius = 10;	/* start with a radius of 10    */
	da = 360;	/* delta-angle for spiral steps */

	/* NOTE: GEM angles are in tenths of a degree */

	vswr_mode(handle, MD_REPLACE);	/* make sure we're in replace mode */

	/* vswr_mode sets writing mode to one of 4 */

	vsl_color(handle, 2);		/* reset color */
	vsl_type(handle, SOLID);	/* and line type */
	vsl_width(handle, 3);		/* use fairly thick lines */

	/* now draw the spiral */

	for (revs = 0; revs < 6; revs++) {

		for (ang = 0; ang < 3600; ang += da) {

			v_arc(handle, 190, 160, radius, ang, ang+da);
			radius++;
		}
	}

	vsl_color(handle, BLACK);	/* reset color */
	vsl_type(handle, SOLID);	/* and line type */
	vsl_width(handle, 1);		/* reset line width */
}

pie_test()
{
	int nsegs, da, ang, seg;

	vsf_interior(handle, HATCH);

	/* making pies has never been easier */

	nsegs = 12;		/* No. of segments in pie */
	da = 3600 / nsegs;	/* angle for each segment */

	for (seg = 0; seg < nsegs; seg++) {

		ang = seg * da;
		vsf_style(handle, seg+2);
		v_pieslice(handle, 472, 100, 90, ang, ang+da);
	}
}

/* 
*/

text_test()   
{

	int s;		/* somewhere to put returned values */

#if	HASLDFNT
	s = vst_load_fonts(handle, 0);
	printf("%d extra fonts loaded.\n", s);
#endif

	vswr_mode(handle, MD_TRANS);	/* writing mode so only fg drawn */
	vst_height(handle, 50, &dummy, &dummy, &dummy, &dummy);  /* BIG */
	s = vst_color(handle, BLACK);   /* text color */
	v_gtext(handle, 390, 100, "Metacomco!");	/* print text at x,y */

	vswr_mode(handle ,MD_REPLACE);		/* change writing mode to REPLACE */
	vst_height(handle, 100, &dummy, &dummy, &dummy, &dummy);	/* BIGGER */
	s = vst_color(handle, RED);   /* text color */
	s = vst_effects(handle, SKEWED);	/* make it skewed */
	v_justified(handle, 380, 155, "Hi There!", 180, 1, 1);

	/* justified text is fill justified to required length (180) */
}

fill_test()
{
	vsf_interior(handle, HATCH);
	vsf_style(handle, GRID);
	vsf_color(handle, 3);
	v_contourfill(handle, 5, 5, -1);

	/*
		that last value is the boundary color.
		-1 implies any color change is to be
		regarded as a boundary condition
	*/
}

gem_close()
{

	/* anything to do just before finishing should go here */

	v_clsvwk(handle);	/* close virtual workstation */
}

/* 
*/

gem_init()
{
	WORD  i;
	WORD  work_in[11],work_out[57];

	/* NOTE: if the data out from opnvwk() is needed elsewhere */
	/* it may be more convenient to declare it as an */
	/* external static or global */

	appl_init();	/* open the application (tell GEM AES were here) */

	/* find system handle ... */

	phys_handle = graf_handle(&dummy, &dummy, &dummy, &dummy);

	handle = phys_handle;   /* handle will contain our vdi handle */

	/* use this handle for all future VDI calls */

	for (i = 0; i < 10; i++)
		work_in[i] = 1;	/* set all defaults before we open it */

	work_in[10] = 2;	/* use raster co-ordinates */

	v_opnvwk(work_in, &handle, work_out);	/* open the workstation */

	v_hide_c(handle);	/* hide the mouse */
	v_clrwk(handle);	/* clear workstation */
	v_show_c(handle, 1);	/* show the mouse again */

	/* turn clipping on to stop screen overflow... */

	setpxy(0, 0, 639, 399);
	vs_clip(handle, TRUE, pxyarray);	/* clip full screen */
}

/* 
*/

wait4it()
{
	WORD c;

	putchar(7);	/* ring bell */

	do {

		vq_mouse(handle, &c, &dummy, &dummy);

		/* reads mouse buttons (bits 0 & 1), xpos, ypos respect. */

	} while( !c );		/* loop until button pressed */
}

main()
{
	gem_init();		/* initialise a virtual workstation */

	box_test();		/* test assorted square primitives  */
	line_test();		/* test polyline and polymarker f's */
	wait4it();		/* wait for a mouse button */

	arc_test();		/* test arc functions */
	circ_test();		/* test circle & ellipse primitives */
	pie_test();		/* test pie segment functions */
	wait4it();		/* wait for a mouse button */

	text_test();		/* test text related GEM functions  */
	fill_test();		/* test contour fill op */
	wait4it();		/* wait for a mouse button */

	gem_close();		/* close up the workstation before we exit */
}
