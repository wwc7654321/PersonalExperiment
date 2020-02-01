#include <stdio.h>
#include <string.h>

#include "MyGeometry2DBase.h"
#include "MyGeometry3DBase.h"
#include "Raytrace.h"
typedef MyGeo2D::Geo2D<int> Geo2D;
typedef Geo2D::Point Point;
typedef Geo2D::Line Line;
typedef Geo2D::Vector Vector;

typedef MyGeo3D::Geo3D<int> Geo3D;
typedef Geo3D::Point Point3Di;
typedef Geo3D::Line Line3Di;
typedef Geo3D::Vector Vector3Di;
typedef Geo3D::Surface Surface3Di;

typedef MyGeo3D::Geo3D<float> Geo3Df;
typedef Geo3Df::Point Point3Df;
typedef Geo3Df::Line Line3Df;
typedef Geo3Df::Vector Vector3Df;
typedef Geo3Df::Surface Surface3Df;

typedef MyGeo2D::Geo2D<float> Geo2Df;
typedef Geo2Df::Point Pointf;
typedef Geo2Df::Line Linef;
typedef Geo2Df::Vector Vectorf; 

void Print(const Pointf & p)
{
	printf("(%f,%f)", p.X(), p.Y());
}
void Print(const Point3Df & p)
{
	printf("(%f,%f,%f)", p.X(), p.Y(), p.Z());
}
void Print(const Point3Di & p)
{
	printf("(%d,%d,%d)", p.X(), p.Y(), p.Z());
}
void Print(const Line3Di & p)
{
	Print(p.P1());
	putchar('-');
	Print(p.P2());
}


void gen1() {
	Point3Df p1(0, 2, 0), p2(2, 0, 0), p3(1, 0, 0), p4(3, 2, 0);
	Vector3Df v1(p1, p3), v2(p1, p4), v3(p2, p3), v4(p2, p4);
	float f1 = v1.Cross(v2).Z(), f2 = v3.Cross(v4).Z();
	printf("%f:%f =%f", f1, f2, f1 / f2); 
}

bool Gen(float a, float b, float c, float d, float e, float f,float &x,float &y) {
	
	if (a*e == b*d)return false;
	x = (b*f - e*c) / (a*e - b*d);
	y = (a*f - d*c) / (b*d - a*e);
	return true;
}
Point3Df gen(Point3Df p1, Point3Df p2, Point3Df p3, Point3Df p4) {
	Vector3Df u(p1, p2), v(p3, p4), t(p4,p2);
	int i = 0;
	float x, y;
	if (Gen(u.X(), v.X(), t.X(), u.Y(), v.Y(), t.Y(), x, y))
	{ 
		Point3Df p = u*x + p2;

		if ( p== p4-v*y )
		{
			return p;
		}
		else return Point3Df();
	}
	if (Gen(u.Y(), v.Y(), t.Y(), u.Z(), v.Z(), t.Z(), x, y))
	{
		Point3Df p = u*x + p2;

		if (p == p4 - v*y)
		{
			return p;
		}
		else return Point3Df();
	}
	return Point3Df();
}


unsigned char *data = 0;
unsigned char *getData()
{
	//memset(data, 0, sizeof(data));
	return data;
}
int getWidth()
{
	return 600;

}
int getHeight()
{
	return 480;
}
int getStride()
{
	return getWidth()*4;
}
#define  TEST_BY_GTK3
#ifdef TEST_BY_GTK2
#define goffset int
#include <cairo.h>
#include <gtk/gtk.h>
typedef gboolean (*pFunGEvent)(GtkWidget *widget, GdkEventExpose *event, gpointer data);
int gtkWindow(pFunGEvent exposeFunc, gint(*pFuncTimer)(gpointer data), int iPWidth, int iPHeight, char * title=0, int argc=0, char ** argv=0);
gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data);
gint timer(gpointer data);

int TestGTK() {
	data = new unsigned char[getStride()*getHeight()];
	memset(data, 0, getStride()*getHeight());
	//Sphere<int> sss;
	Ray3D<int> rrr;
	GeometryShape<int>::IntersectPoint ip;
	//sss.intersect(rrr, ip);
	Point3Di pp;
	Surface3Di sur;
	Vector3Di vect(1,3,5),vecto(2,3,4);
	Point p1(200,200);
	Vector vec;
	Line l1(200, 300, 400, 500);
	vec.X(100);
	p1 += vec;
	vect *= 2; 
	vect += vecto;
	Print(vect);
	sur.PointOnSurface(pp); 
	printf("%d,%d  %d\n",p1.X(),p1.Y(),p1.Dist(l1));
	printf("200,300 %d\n",l1.PointOnLine(Point(200,300)));
	printf("200,300 %d\n", l1.PointOnSegLine(Point(200, 300)));

	printf("300,300 %d\n", l1.PointOnLine(Point(300, 300)));
	printf("300,300 %d\n", l1.PointOnSegLine(Point(300, 300)));

	printf("600,700 %d\n", l1.PointOnLine(Point(600, 700)));
	printf("600,700 %d\n", l1.PointOnSegLine(Point(600, 700)));

	//p1 = vec;
	//vec = p1;
	Point3Df po1(0, 2, 0), po2(2, 0, 0), po3(1, 0, 0), po4(3, 2, 0);
	Point3Df p = Line3Df(po1,po2).CrossAt(Line3Df(po3,po4));
	if (p.IsValid()) Print(p);
	else printf("no point");
	putchar('\n');


	Pointf poi1(0, 2), poi2(2, 0), poi3(1, 0), poi4(3, 2);
	Pointf pi = Linef(poi1, poi2).CrossAt(Linef(poi3, poi4));
	if (pi.IsValid()) Print(pi);
	else printf("no point");
	putchar('\n');
	//getchar();
	gtkWindow(&on_expose_event, &timer, 600, 480, "Test1");
	 
	return 0;
}


int gtkWindow(pFunGEvent exposeFunc, gint (*pFuncTimer)(gpointer data),int iPWidth,int iPHeight,char * title/*=0*/,int argc/*=0*/,char ** argv/*=0*/)
{
	GtkWidget *window;
	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(window, "expose-event", G_CALLBACK(exposeFunc), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), iPWidth, iPHeight);
	if (title != 0) gtk_window_set_title(GTK_WINDOW(window), title);

	int sign = g_timeout_add(1000, GSourceFunc( pFuncTimer), 0);

	gtk_widget_set_app_paintable(window, TRUE);
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}

cairo_t* GetCairo(GtkWidget* widget)
{
	cairo_t* cr;
	cr = gdk_cairo_create(widget->window);
	return cr;
}

gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	auto cr = GetCairo(widget);
	cairo_surface_t *image = cairo_image_surface_create_from_png("star.png");
	cairo_format_t f=cairo_image_surface_get_format(image);
	int w=cairo_image_surface_get_width(image);
	int h=cairo_image_surface_get_height(image);
	int s=cairo_image_surface_get_stride(image); 
	unsigned char * d = getData();//cairo_image_surface_get_data(image);
	
 
	cairo_surface_t *imgs=cairo_image_surface_create_for_data(d, CAIRO_FORMAT_RGB24, w, h, s);
	/*cairo_set_source_surface(cr, imgs, 0, 0);
	cairo_paint(cr); */


	cr = GetCairo(widget);
	cairo_set_source_surface(cr, imgs, 10, 10);

	/*cairo_surface_t *suf;
	cairo_t *suft;
	suf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 40, 40);
	suft = cairo_create(suf);
	cairo_set_source_surface(suft, imgs, 0, 0);
	cairo_paint(suft);
	//cairo_surface_write_to_png(suf, "image.png");

	cairo_set_source_surface(cr, suf, 10, 10  );
	cairo_paint(cr);
	cairo_set_source_surface(cr, suf, 30, 30);
*/
	cairo_paint(cr);
	cairo_destroy(cr);
	return FALSE;
	return true;
}
gint timer(gpointer data)
{
	printf("a");
	return TRUE;
}
#else
#ifdef TEST_BY_GTK3
#include <cairo.h>
#include <gtk/gtk.h>
typedef gboolean(*pFunGEvent)(GtkWidget* widget, cairo_t* cr, gpointer user_data);
int gtkWindow(pFunGEvent exposeFunc, gint(*pFuncTimer)(gpointer data), int iPWidth, int iPHeight, char* title = 0, int argc = 0, char** argv = 0);
gboolean on_draw(GtkWidget* widget, cairo_t* cr, gpointer user_data);
gint timer(gpointer data);

int TestGTK() {
	data = new unsigned char[getStride() * getHeight()];
	memset(data, 0, getStride() * getHeight());
	//Sphere<int> sss;
	Ray3D<int> rrr;
	GeometryShape<int>::IntersectPoint ip;
	//sss.intersect(rrr, ip);
	Point3Di pp;
	Surface3Di sur;
	Vector3Di vect(1, 3, 5), vecto(2, 3, 4);
	Point p1(200, 200);
	Vector vec;
	Line l1(200, 300, 400, 500);
	vec.X(100);
	p1 += vec;
	vect *= 2;
	vect += vecto;
	Print(vect);
	sur.PointOnSurface(pp);
	printf("%d,%d  %d\n", p1.X(), p1.Y(), p1.Dist(l1));
	printf("200,300 %d\n", l1.PointOnLine(Point(200, 300)));
	printf("200,300 %d\n", l1.PointOnSegLine(Point(200, 300)));

	printf("300,300 %d\n", l1.PointOnLine(Point(300, 300)));
	printf("300,300 %d\n", l1.PointOnSegLine(Point(300, 300)));

	printf("600,700 %d\n", l1.PointOnLine(Point(600, 700)));
	printf("600,700 %d\n", l1.PointOnSegLine(Point(600, 700)));

	//p1 = vec;
	//vec = p1;
	Point3Df po1(0, 2, 0), po2(2, 0, 0), po3(1, 0, 0), po4(3, 2, 0);
	Point3Df p = Line3Df(po1, po2).CrossAt(Line3Df(po3, po4));
	if (p.IsValid()) Print(p);
	else printf("no point");
	putchar('\n');


	Pointf poi1(0, 2), poi2(2, 0), poi3(1, 0), poi4(3, 2);
	Pointf pi = Linef(poi1, poi2).CrossAt(Linef(poi3, poi4));
	if (pi.IsValid()) Print(pi);
	else printf("no point");
	putchar('\n');
	//getchar();
	gtkWindow(&on_draw, &timer, 600, 480, "Test1");

	return 0;
}


int gtkWindow(pFunGEvent exposeFunc, gint(*pFuncTimer)(gpointer data), int iPWidth, int iPHeight, char* title/*=0*/, int argc/*=0*/, char** argv/*=0*/)
{
	GtkWidget* window;
	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(window, "draw", G_CALLBACK(exposeFunc), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), iPWidth, iPHeight);
	if (title != 0) gtk_window_set_title(GTK_WINDOW(window), title);

	int sign = g_timeout_add(1000, GSourceFunc(pFuncTimer), 0);

	gtk_widget_set_app_paintable(window, TRUE);
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}

cairo_t* GetCairo(GtkWidget* widget)
{
	cairo_t* cr; 
	auto w = gtk_widget_get_window(widget);
	auto cont = gdk_window_begin_draw_frame(w, nullptr);
	cr = gdk_drawing_context_get_cairo_context(cont); 
	return cr;
}

gboolean on_draw(GtkWidget* widget, cairo_t* cr, gpointer user_data)
{
	//auto cr = GetCairo(widget);
	cairo_surface_t* image = cairo_image_surface_create_from_png("star.png");
	cairo_format_t f = cairo_image_surface_get_format(image);
	int w = cairo_image_surface_get_width(image);
	int h = cairo_image_surface_get_height(image);
	int s = cairo_image_surface_get_stride(image);
	unsigned char* d = cairo_image_surface_get_data(image);


	cairo_surface_t* imgs = cairo_image_surface_create_for_data(d, CAIRO_FORMAT_ARGB32, w, h, s);
	/*cairo_set_source_surface(cr, imgs, 0, 0);
	cairo_paint(cr); */


	//cr = GetCairo(widget);
	//cairo_set_source_surface(cr, imgs, 10, 10);

	cairo_surface_t *suf;
	cairo_t *suft;
	suf = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 40, 40);
	suft = cairo_create(suf);
	cairo_set_source_surface(suft, imgs, 0, 0);
	cairo_paint(suft);
	//cairo_surface_write_to_png(suf, "image.png");

	cairo_set_source_surface(cr, suf, 100, 100  );
	cairo_paint(cr);
	cairo_set_source_surface(cr, suf, 300, 300);

	cairo_paint(cr);
	//cairo_destroy(cr);
	return FALSE;
}
gint timer(gpointer data)
{
	printf("a");
	return TRUE;
}
#else
int TestGTK() {
	return 0;
}
#endif
#endif

int main()
{
	TestGTK();
	return 0;
}