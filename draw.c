#include <stdio.h>
#include <stdlib.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "math.h"

/*======== void add_polygon() ==========
  Inputs:   struct matrix *surfaces
  double x0
  double y0
  double z0
  double x1
  double y1
  double z1
  double x2
  double y2
  double z2  
  Returns: 
  Adds the vertices (x0, y0, z0), (x1, y1, z1)
  and (x2, y2, z2) to the polygon matrix. They
  define a single triangle surface.
  ====================*/
void add_polygon( struct matrix *polygons, 
                  double x0, double y0, double z0, 
                  double x1, double y1, double z1, 
                  double x2, double y2, double z2 ) {
  add_point(polygons, x0, y0, z0);
  add_point(polygons, x1, y1, z1);
  add_point(polygons, x2, y2, z2);
}

/*======== void draw_polygons() ==========
  Inputs:   struct matrix *polygons
  screen s
  color c  
  Returns: 
  Goes through polygons 3 points at a time, drawing 
  lines connecting each points to create bounding
  triangles
  ====================*/
void draw_polygons( struct matrix *polygons, screen s, color c ) {
  int ax, ay, bx, by;
  for(int i = 0; i < polygons ->lastcol; i += 3){
    ax = polygons->m[0][i + 1] - polygons->m[0][i];
    ay = polygons->m[1][i + 1] - polygons->m[1][i];
    bx = polygons->m[0][i + 2] - polygons->m[0][i];
    by = polygons->m[1][i + 2] - polygons->m[1][i];

    int cross = ax*by - ay*bx;
    if(cross > 0){
      draw_line(polygons->m[0][i],
		polygons->m[1][i],
		polygons->m[0][i + 1],
		polygons->m[1][i + 1],s, c);
      draw_line(polygons->m[0][i + 1],
		polygons->m[1][i + 1],
		polygons->m[0][i + 2],
		polygons->m[1][i + 2],s, c);
      draw_line(polygons->m[0][i + 2],
		polygons->m[1][i + 2],
		polygons->m[0][i],
		polygons->m[1][i],s, c);
    }
		
  }
}


  /*======== void add_box() ==========
    Inputs:   struct matrix * edges
    double x
    double y
    double z
    double width
    double height
    double depth
    Returns: 

    add the points for a rectagular prism whose 
    upper-left corner is (x, y, z) with width, 
    height and depth dimensions.
    ====================*/
  void add_box( struct matrix *edges,
		double x, double y, double z,
		double width, double height, double depth ) {

    double x0, y0, z0, x1, y1, z1;
    x0 = x;
    x1 = x+width;
    y0 = y;
    y1 = y-height;
    z0 = z;
    z1 = z-depth;

  
    //front face
    add_polygon(edges, x0, y0, z0, x1, y1, z0, x1, y0, z0);
    add_polygon(edges, x0, y0, z0, x0, y1, z0, x1, y1, z0);     
    //back faced
    add_polygon(edges, x1, y, z1, x, y1, z1, x, y, z1);
    add_polygon(edges, x1, y, z1, x1, y1, z1, x, y1, z1); 
    //left frace
    add_polygon(edges, x, y, z1, x, y1, z, x, y, z); 
    add_polygon(edges, x, y, z1, x, y1, z1, x, y1, z);
    //right farsi
    add_polygon(edges, x1, y, z, x1, y1, z1, x1, y, z1); 
    add_polygon(edges, x1, y, z, x1, y1, z, x1, y1, z1);
    //top facey
    add_polygon(edges, x, y, z1, x1, y, z, x1, y, z1); 
    add_polygon(edges, x, y, z1, x, y, z, x1, y, z);
    //bottom faco
    add_polygon(edges, x, y1, z, x1, y1, z1, x1, y1, z);
    add_polygon(edges, x, y1, z, x, y1, z1, x1, y1, z1);
  }



  /*======== void add_sphere() ==========
    Inputs:   struct matrix * points
    double cx
    double cy
    double cz
    double r
    int step  
    Returns: 

    adds all the points for a sphere with center 
    (cx, cy, cz) and radius r.

    should call generate_sphere to create the
    necessary points
    ====================*/
  void add_sphere( struct matrix * polygons, 
		   double cx, double cy, double cz,
		   double r, int step ) {

    struct matrix *points = generate_sphere(cx, cy, cz, r, step);
    int index, lat, longt, next;
    int latStop, longStop, latStart, longStart;
    latStart = 0;
    latStop = step;
    longStart = 0;
    longStop = step;

    step++;
    for ( lat = latStart; lat < latStop; lat++ ) {
      for ( longt = longStart; longt < longStop; longt++ ) {
	index = lat * step + longt;
      
	if(lat < latStop - 1)
	  next = (lat+1) * (step) + longt;
	else
	  next = longt;

	if(longt < longStop) {	
	  add_polygon(polygons,
		      points->m[0][next],
		      points->m[1][next],
		      points->m[2][next],
		      points->m[0][index+1],
		      points->m[1][index+1],
		      points->m[2][index+1],
		      points->m[0][index],
		      points->m[1][index],
		      points->m[2][index]);		      
	  add_polygon(polygons,
		      points->m[0][next],
		      points->m[1][next],
		      points->m[2][next],
		      points->m[0][next+1],
		      points->m[1][next+1],
		      points->m[2][next+1],
		      points->m[0][index+1],
		      points->m[1][index+1],
		      points->m[2][index+1]);
	}
      }
    }
  
  free_matrix(points);
}

/*======== void generate_sphere() ==========
  Inputs:   double cx
	    double cy
	    double cz
	    double r
	    double step  
  Returns: Generates all the points along the surface 
           of a sphere with center (cx, cy, cz) and
	   radius r.
	   Returns a matrix of those points
  ====================*/
struct matrix * generate_sphere(double cx, double cy, double cz,
                                double r, int step ) {

  struct matrix *points = new_matrix(4, step * step);
  int circle, rotation, rot_start, rot_stop, circ_start, circ_stop;
  double x, y, z, rot, circ;

  rot_start = 0;
  rot_stop = step;
  circ_start = 0;
  circ_stop = step;

  for (rotation = rot_start; rotation < rot_stop; rotation++) {
    rot = (double)rotation / step;

    for(circle = circ_start; circle <= circ_stop; circle++){
      circ = (double)circle / step;

      x = r * cos(M_PI * circ) + cx;
      y = r * sin(M_PI * circ) *
        cos(2*M_PI * rot) + cy;
      z = r * sin(M_PI * circ) *
        sin(2*M_PI * rot) + cz;

      /* printf("rotation: %d\tcircle: %d\n", rotation, circle); */
      /* printf("rot: %lf\tcirc: %lf\n", rot, circ); */
      /* printf("sphere point: (%0.2f, %0.2f, %0.2f)\n\n", x, y, z); */
      add_point(points, x, y, z);
    }
  }

  return points;
}

/*======== void add_torus() ==========
  Inputs:   struct matrix * points
            double cx
	    double cy
	    double cz
	    double r1
	    double r2
	    double step  
  Returns: 

  adds all the points required to make a torus
  with center (cx, cy, cz) and radii r1 and r2.

  should call generate_torus to create the
  necessary points
  ====================*/
void add_torus( struct matrix * polygons, 
                double cx, double cy, double cz,
                double r1, double r2, int step ) {

  struct matrix *points = generate_torus(cx, cy, cz, r1, r2, step);
  int index, adex, bdex, next, lat, longt;
  int latStop, longStop, latStart, longStart;
  latStart = 0;
  latStop = step;
  longStart = 0;
  longStop = step;

  for ( lat = latStart; lat < latStop; lat++ ) {
    for ( longt = longStart; longt < longStop; longt++ ) {
      index = lat * step + longt;

      adex = (index + 1) % (latStop * longStop);
      next = (index + longStop) % (latStop * longStop);
      bdex = (index + longStop + 1) % (latStop * longStop);
      
      if(longt < longStop) {	
	add_polygon(polygons,
		    points->m[0][index],
		    points->m[1][index],
		    points->m[2][index],
		    points->m[0][adex],
		    points->m[1][adex],
		    points->m[2][adex],
		    points->m[0][next],
		    points->m[1][next],
		    points->m[2][next]);		      
	add_polygon(polygons,
		    points->m[0][adex],
		    points->m[1][adex],
		    points->m[2][adex],
		    points->m[0][bdex],
		    points->m[1][bdex],
		    points->m[2][bdex],
		    points->m[0][next],
		    points->m[1][next],
		    points->m[2][next]);
      }
    }
  }
  
  free_matrix(points);
}

/*======== void generate_torus() ==========
  Inputs:   struct matrix * points
            double cx
	    double cy
	    double cz
	    double r
	    int step  
  Returns: Generates all the points along the surface 
           of a torus with center (cx, cy, cz) and
	   radii r1 and r2.
	   Returns a matrix of those points
  ====================*/
struct matrix * generate_torus( double cx, double cy, double cz,
                                double r1, double r2, int step ) {

  struct matrix *points = new_matrix(4, step * step);
  int circle, rotation, rot_start, rot_stop, circ_start, circ_stop;
  double x, y, z, rot, circ;

  rot_start = 0;
  rot_stop = step;
  circ_start = 0;
  circ_stop = step;

  for (rotation = rot_start; rotation < rot_stop; rotation++) {
    rot = (double)rotation / step;

    for(circle = circ_start; circle < circ_stop; circle++){
      circ = (double)circle / step;

      x = cos(2*M_PI * rot) *
        (r1 * cos(2*M_PI * circ) + r2) + cx;
      y = r1 * sin(2*M_PI * circ) + cy;
      z = -1*sin(2*M_PI * rot) *
        (r1 * cos(2*M_PI * circ) + r2) + cz;

      //printf("rotation: %d\tcircle: %d\n", rotation, circle);
      //printf("torus point: (%0.2f, %0.2f, %0.2f)\n", x, y, z);
      add_point(points, x, y, z);
    }
  }
  
  return points;
}

/*======== void add_circle() ==========
  Inputs:   struct matrix * points
            double cx
	    double cy
	    double r
	    double step  
  Returns: 

  Adds the circle at (cx, cy) with radius r to edges
  ====================*/
void add_circle( struct matrix * edges, 
                 double cx, double cy, double cz,
                 double r, int step ) { 
  double x0, y0, x1, y1, t;
  int i;
  x0 = r + cx;
  y0 = cy;

  for (i=1; i<=step; i++) {
    t = (double)i/step;
    x1 = r * cos(2 * M_PI * t) + cx;
    y1 = r * sin(2 * M_PI * t) + cy;

    add_edge(edges, x0, y0, cz, x1, y1, cz);
    x0 = x1;
    y0 = y1;
  }
}


/*======== void add_curve() ==========
Inputs:   struct matrix *points
         double x0
         double y0
         double x1
         double y1
         double x2
         double y2
         double x3
         double y3
         double step
         int type  
Returns: 

Adds the curve bounded by the 4 points passsed as parameters
of type specified in type (see matrix.h for curve type constants)
to the matrix points
====================*/
void add_curve( struct matrix *edges, 
                double x0, double y0, 
                double x1, double y1, 
                double x2, double y2, 
                double x3, double y3, 
                int step, int type ) {

  double t, x, y;
  struct matrix *xcoefs;
  struct matrix *ycoefs;
  int i;
  
  xcoefs = generate_curve_coefs(x0, x1, x2, x3, type);
  ycoefs = generate_curve_coefs(y0, y1, y2, y3, type);

  /* print_matrix(xcoefs); */
  /* printf("\n"); */
  /* print_matrix(ycoefs); */

  for (i=1; i<=step; i++) {

    t = (double)i/step;
    x = xcoefs->m[0][0] *t*t*t + xcoefs->m[1][0] *t*t+
      xcoefs->m[2][0] *t + xcoefs->m[3][0];
    y = ycoefs->m[0][0] *t*t*t + ycoefs->m[1][0] *t*t+
      ycoefs->m[2][0] *t + ycoefs->m[3][0];
    
    add_edge(edges, x0, y0, 0, x, y, 0);
    x0 = x;
    y0 = y;
  }
  
  free_matrix(xcoefs);
  free_matrix(ycoefs);
}

/*======== void add_point() ==========
  Inputs:   struct matrix * points
  int x
  int y
  int z 
  Returns: 
  adds point (x, y, z) to points and increment points.lastcol
  if points is full, should call grow on points
  ====================*/
void add_point( struct matrix * points, double x, double y, double z) {

  if ( points->lastcol == points->cols )
    grow_matrix( points, points->lastcol + 100 );
  
  points->m[0][ points->lastcol ] = x;
  points->m[1][ points->lastcol ] = y;
  points->m[2][ points->lastcol ] = z;
  points->m[3][ points->lastcol ] = 1;
  points->lastcol++;
} //end add_point

/*======== void add_edge() ==========
  Inputs:   struct matrix * points
  int x0, int y0, int z0, int x1, int y1, int z1
  Returns: 
  add the line connecting (x0, y0, z0) to (x1, y1, z1) to points
  should use add_point
  ====================*/
void add_edge( struct matrix * points, 
               double x0, double y0, double z0, 
               double x1, double y1, double z1) {
  add_point( points, x0, y0, z0 );
  add_point( points, x1, y1, z1 );
}

/*======== void draw_lines() ==========
  Inputs:   struct matrix * points
  screen s
  color c 
  Returns: 
  Go through points 2 at a time and call draw_line to add that line
  to the screen
  ====================*/
void draw_lines( struct matrix * points, screen s, color c) {

  if ( points->lastcol < 2 ) {
    return;
  }
 
  int point;
  for (point=0; point < points->lastcol-1; point+=2)
    draw_line( points->m[0][point],
               points->m[1][point],
               points->m[0][point+1],
               points->m[1][point+1],
               s, c);	       
}// end draw_lines


void draw_line(int x0, int y0, int x1, int y1, screen s, color c) {
  
  int x, y, d, A, B;
  //swap points if going right -> left
  int xt, yt;
  if (x0 > x1) {
    xt = x0;
    yt = y0;
    x0 = x1;
    y0 = y1;
    x1 = xt;
    y1 = yt;
  }
  
  x = x0;
  y = y0;
  A = 2 * (y1 - y0);
  B = -2 * (x1 - x0);  

  //octants 1 and 8
  if ( abs(x1 - x0) >= abs(y1 - y0) ) {

    //octant 1    
    if ( A > 0 ) {
      
      d = A + B/2;      
      while ( x < x1 ) {
        plot( s, c, x, y );
        if ( d > 0 ) {
          y+= 1;
          d+= B;
        }
        x++;
        d+= A;
      } //end octant 1 while
      plot( s, c, x1, y1 );
    } //end octant 1

    //octant 8
    else {
      d = A - B/2;
      
      while ( x < x1 ) {
        //printf("(%d, %d)\n", x, y);
        plot( s, c, x, y );
        if ( d < 0 ) {
          y-= 1;
          d-= B;
        }
        x++;
        d+= A;
      } //end octant 8 while
      plot( s, c, x1, y1 );
    } //end octant 8
  }//end octants 1 and 8

  //octants 2 and 7
  else {
    
    //octant 2    
    if ( A > 0 ) {
      d = A/2 + B;      

      while ( y < y1 ) {
        plot( s, c, x, y );
        if ( d < 0 ) {
          x+= 1;
          d+= A;
        }
        y++;
        d+= B;
      } //end octant 2 while
      plot( s, c, x1, y1 );
    } //end octant 2

    //octant 7
    else {
      d = A/2 - B;
      
      while ( y > y1 ) {
        plot( s, c, x, y );
        if ( d > 0 ) {
          x+= 1;
          d+= A;
        }
        y--;
        d-= B;
      } //end octant 7 while
      plot( s, c, x1, y1 );
    } //end octant 7   
  }//end octants 2 and 7  
} //end draw_line
