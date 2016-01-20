 
#include "oldmyVector.h"
 


Point3::Point3()
{
 x=0;
 y=0;
 z=0;
}
Point3::Point3(const Point3 & a)
{
 x=a.x;
 y=a.y;
 z=a.z;
}
Point3::Point3(t_double xx,t_double yy,t_double zz)
{
 x=xx;
 y=yy;
 z=zz;
}

t_double Point3::awayfrom(const Point3 & a)const
{
	return sqrt((x-a.x)*(x-a.x) + (y-a.y)*(y-a.y) + (z-a.z)*(z-a.z));	
}
t_double Point3::awayfrom(const Line3 & a)const
{
	return a.awayfrom (*this) ;
}
t_double Point3::awayfrom(const Surface3 & a)const
{
	return a.awayfrom(*this) ;
}
Point3 Point3::operator+(const Vector3 &a)const
{
	Point3 tmp;
	tmp.x=x+a.x;
	tmp.y=y+a.y;
	tmp.z=z+a.z;
	return tmp;
}
Point3& Point3::operator+=(const Vector3 &a)
{
	x+=a.x;
	y+=a.y;
	z+=a.z;
	return *this;
}
bool Point3::operator==(const Point3 &a)const
{
	return x==a.x&&
	y==a.y&&
	z==a.z ;
}
Point3& Point3::operator=(const Point3 &a)
{
	x=a.x;
	y=a.y;
	z=a.z;
	return *this;
}
Point3& Point3::operator=(const Vector3 &a)
{
	x=a.x;
	y=a.y;
	z=a.z;
	return *this;
}
Point3& Point3::operator-(const Vector3 &a)const
{
	static Point3 tmp;
	tmp.x=x-a.x;
	tmp.y=y-a.y;
	tmp.z=z-a.z;
	return tmp;
}

Point3& Point3::operator-=(const Vector3 &a)
{
	x-=a.x;
	y-=a.y;
	z-=a.z;
	return *this;
}
Vector3& Point3::tovector()const
{
	static Vector3 v;
	v.x =x;
	v.y=y;
	v.z=z;
	return v;
}
Line3 & Point3::lineto(const Point3 & a)const
{
	static Line3 tmp;
	tmp.set(*this,a);
	//tmp.x=a.x-x;
//	tmp.y=a.y-y;
//	tmp.z=a.z-z;
	return  tmp;	
}
Point3 & Point3::midpointto(const Point3& p2)const
{
	static Point3 p;
	p.x=(x+p2.x)/2;
	p.y=(y+p2.y)/2;
	p.z=(z+p2.z)/2;
	return p;
}










Line3::Line3()
{
		
}
Line3::Line3(t_double x,t_double y)
{
	set(0,0,x,y);
}

Line3::Line3(t_double x,t_double y,const Vector3& v)
{
	set(x,y,v);
}

Line3::Line3(const Point3& p11,const Point3& p22)
{
	set(p11,p22);
}
Line3::Line3(const Point3& p11,const Vector3& v)
{
	set(p11,v);
}
Line3::Line3(t_double x1,t_double y1,t_double x2,t_double y2)
{
	set(x1,y1,x2,y2);
}


Vector3 & Line3::getvector()const 
{
	static Vector3 tmp;
	tmp.x=p2.x-p1.x;
	tmp.y=p2.y-p1.y;
	tmp.z=p2.z-p1.z;
	return tmp;
}

bool  Line3::ape (const Line3 & a)const 
{
	return getvector() .ape (a.getvector ());
}
bool  Line3::par (const Line3 & a)const 
{
	return getvector() .par (a.getvector ());
}

bool  Line3::ape (const Surface3 & a)const 
{
	return a.ape (*this);
}
bool  Line3::par (const Surface3 & a)const 
{
	return a.par(*this);
}

bool  Line3::ape (const Vector3 & a)const 
{
	return getvector() .ape (a);
}
bool  Line3::par (const Vector3 & a)const 
{
	return getvector() .par (a);
}



t_double Line3::awayfrom(const Point3 & a)const
{
	Vector3 v1(p1,p2), v2(p1,a); 
	
	return (v1^v2).length()/v1.length() ;	
}
t_double Line3::awayfrom(const Line3 & a)const
{
	if (a.par(*this))return awayfrom(a.p1 );
	Vector3 v1(a.p1 ,p1);
	return v1.getunit() *(a.getvector ()^getvector());
}
t_double Line3::awayfrom(const Surface3 & a)const
{
	return a.awayfrom(*this) ;
}


void Line3::set(t_double x,t_double y,const Vector3& v)
{
	p1.x=x;
	p1.y=y;
	p2=p1+v;
}

void Line3::set(const Point3& p11,const Point3& p22)
{
	p1=p11;
	p2=p22;
}
void Line3::set(const Point3& p11,const Vector3& v)
{
	p1=p11;
	p2=p1+v;
}
void Line3::set(t_double x1,t_double y1,t_double x2,t_double y2)
{

	p1.x=x1;
	p1.y=y1;
	p2.x=x2;
	p2.y=y2;
}



bool     Line3::operator==(const Line3 &a)const
{ 
	return a.par(*this)&& awayfrom(a.p1 )==0;
}

Line3 & Line3::operator+ (const Vector3 &a)const
{
	static Line3 tmp;
//	Vector3 & v1=getvector();
//	if(v1.par(a))return *this;
	tmp.p1= p1+a;
	tmp.p2=p2+a;
	return tmp;
}
Line3 & Line3::operator+=(const Vector3 &a)
{
//	Vector3 & v1=getvector();
//	if(v1.par(a))return *this;
	p1+=a;
	p2+=a;
	return *this;
}
Line3 & Line3::operator- (const Vector3 &a)const
{
	static Line3 tmp;
//	Vector3 & v1=getvector();
//	if(v1.par(a))return *this;
	tmp.p1=p1-a;
	tmp.p2=p2-a;
	return tmp;
}
Line3 & Line3::operator-=(const Vector3 &a)
{
//	Vector3 & v1=getvector();
//	if(v1.par(a))return *this;
	p1-=a;
	p2-=a;
	return *this;
}
Line3 & Line3::operator=(const Line3 &a)
{
	p1=a.p1;
	p2=a.p2;
	return *this;
}


Point3  Line3::getp1 () const
{
return p1;
}
Point3  Line3::getp2 () const
{
return p2;
}
Point3  Line3::getminpoint ()const
{
return p1.midpointto (p2);
}

















Surface3::Surface3()
{
}
Surface3::Surface3(const Line3 &a,const Line3 &b)
{
	set(a,b);
}
Surface3::Surface3(const Point3 &a,const Vector3 &b)
{
	set(a,b);
/*	if (a.par (b) ) throw 0;
	p1=a.p1 ;
	p2=a.p2;
	p3=a.p1 + b.getvector();*/
}
Surface3::Surface3(const Vector3 &a,const Vector3 &b )
{ 
	set(a,b);
}
Surface3::Surface3(const Vector3 &a,const Vector3 &b,const Point3 &c )
{
	set(a,b,c);
}
Surface3::Surface3(const Point3 &a,const Point3 &b,const Point3 &c)
{
	set(a,b,c);
}

Point3   Surface3::getapoint()const
{

	return Point3  (p1);
}
Vector3 & Surface3::getlawvector()const
{
	static Vector3 v1;
	Vector3 v2;
	v1.set(p1,p2);
	v2.set(p1,p3);
	v1^=v2;
	v1.settounit ();
	return v1;
}
t_double  Surface3::awayfrom(const Point3 &p)const
{
	Vector3 v1(getapoint(),p);
	
	return fabs(getlawvector()* v1);
}
t_double  Surface3::awayfrom(const Line3 &l)const
{
	Vector3 &v1=getlawvector() ;
	if(v1*l.getvector())return 0;
	Vector3 v2(getapoint(),l.getp1());

	return fabs(v1* v2);
}
t_double  Surface3::awayfrom(const Surface3 &s)const
{
	t_double a=awayfrom(s.p2.lineto (s.p3));
	return a?a:awayfrom(s.p1.lineto (s.p2));
}

bool     Surface3::operator==(const Surface3 &a)const
{
	Vector3 &v1=getlawvector() ,&v2=a.getlawvector();

	return v1.par(v2) && awayfrom(a)==0;
}

Surface3 & Surface3::operator+ (const Vector3 &a)const
{
	static Surface3 tmp; 
	tmp.p1= p1+a;
	tmp.p2=p2+a;
	tmp.p3=p3+a;
	return tmp;
}
Surface3 & Surface3::operator+=(const Vector3 &a)
{
 
	p1+=a;
	p2+=a;
	p3+=a;
	return *this;
}
Surface3 & Surface3::operator- (const Vector3 &a)const
{
 static Surface3 tmp;
	tmp.p1=p1-a;
	tmp.p2=p2-a;
	tmp.p3=p3-a;
	return tmp;
}
Surface3 & Surface3::operator-=(const Vector3 &a)
{
	p1-=a;
	p2-=a;
	p3-=a;
	return *this;
}
Surface3 & Surface3::operator=(const Surface3 &a)
{
	p1=a.p1;
	p2=a.p2;
	p3=a.p3;
	return *this;
}

bool  Surface3::ape (const Surface3 & a)const 
{
	return getlawvector() .ape (a.getlawvector ());
}
bool  Surface3::par (const Surface3 & a)const 
{
	return getlawvector() .par (a.getlawvector ());
}

bool  Surface3::ape (const Line3 & a)const 
{
	return a.getvector().par(getlawvector  ());
}
bool  Surface3::par (const Line3 & a)const 
{
	return a.getvector().ape (getlawvector ());
}

bool  Surface3::ape (const Vector3 & a)const 
{
	return a.par(getlawvector  ());
}
bool  Surface3::par (const Vector3 & a)const 
{
	return a.ape (getlawvector ());
}


void Surface3::set(const Line3 &a,const Line3 &b)
{
	if (a.par (b) ) throw 0;
	p1=a.getp1() ;
	p2=a.getp2();
	p3=p1 + b.getvector();
}
void Surface3::set(const Point3 &a,const Vector3 &b)
{
/*	if (a.par (b) ) throw 0;
	p1=a.p1 ;
	p2=a.p2;
	p3=a.p1 + b.getvector();*/
}
void Surface3::set(const Vector3 &a,const Vector3 &b )
{ 
	p2=p1+a;
	p3=p1+b;
}
void Surface3::set(const Vector3 &a,const Vector3 &b,const Point3 &c )
{
	p1=c;
	p2=p1+a;
	p3=p1+b;
}
void Surface3::set(const Point3 &a,const Point3 &b,const Point3 &c)
{
	p1=a;
	p2=b;
	p3=c;
}
 






 
Vector3::Vector3()
{
	x=0;
	y=0;
	z=0;
}
Vector3::Vector3(const Vector3 & a)
{
	x=a.x;
	y=a.y;
	z=a.z;	
}
Vector3::Vector3(const Point3 & a,const Point3 & b)
{
	x=a.x-b.x;
	y=a.y-b.y;
	z=a.z-b.z;	
}
Vector3::Vector3(t_double xx,t_double yy , t_double zz =0)
{
	x=xx;
	y=yy;
	z=zz;
}

bool Vector3::par(const Line3&v1)const 
{
	return v1.par (*this);
}
bool Vector3::ape(const Line3&v1)const 
{
	return  v1.ape (*this) ;
}
bool Vector3::par(const Surface3&v1)const 
{
	return v1.par(*this);
}
bool Vector3::ape(const Surface3&v1)const 
{
	return  v1.ape (*this) ;
}
bool Vector3::par(const Vector3&v1)const 
{
	return x*v1.y==y*v1.x && y * v1.z== z * v1.y;
}
bool Vector3::ape(const Vector3&v1)const 
{
	return  (*this)*v1==0 ;
}
//t_double Vector3::value()
//{
//	return sqrt(x*x+y*y+z*z);	
//}
t_double Vector3::length(void)const
{
	return sqrt(x*x+y*y+z*z);	
}

void Vector3::set(t_double a,t_double b,t_double c)
{
	x=a;
	y=b;
	z=c;
}

void Vector3::set(const Point3& a,const Point3& b)
{
	x=a.x-b.x;
	y=a.y-b.y;
	z=a.z-b.z;
}
void Vector3::set(const Vector3& b)
{
	*this=b;
}

void Vector3::settounit(void)
{
	t_double l=length();
	x/=l;
	y/=l;
	z/=l;
}
Vector3& Vector3::getunit(void)const
{
	static Vector3 v1;
	t_double l=length();
	v1.set(x/l,y/l,z/l);
	return v1;
}



	
Vector3& Vector3::operator=(const Vector3& v1)
{
	x=v1.x;
	y=v1.y;
	z=v1.z;
	return *this;
}
Vector3& Vector3::operator=(const Point3& v1)
{
	x=v1.x;
	y=v1.y;
	z=v1.z;
	return *this;
}

bool  Vector3::operator==(const Vector3&  a)const
{
	return x==a.x && y==a.y && z==a.z ;
}
 
Vector3& Vector3::operator+(const Vector3&v1)const
{
	static Vector3 tmp;
	tmp.x=x+v1.x;
	tmp.y=y+v1.y;
	tmp.z=z+v1.z;
	return tmp ;
}
Vector3& Vector3::operator+=(const Vector3&v1)
{
	x+=v1.x;
	y+=v1.y;
	z+=v1.z;
	return *this ;
}
Vector3& Vector3::operator-(const Vector3&v1)const
{
	static Vector3 tmp;
	tmp.x=x-v1.x;
	tmp.y=y-v1.y;
	tmp.z=z-v1.z;
	return tmp ;
}
Vector3& Vector3::operator-=(const Vector3&v1)
{
	x-=v1.x;
	y-=v1.y;
	z-=v1.z;
	return *this ;
}
Vector3& Vector3::operator-()const
{
	static Vector3 tmp;
	tmp.x=-x;
	tmp.y=-y;
	tmp.z=-z;
	return tmp ;
} 
Vector3& Vector3::operator^(const Vector3&v1)const
{
	static Vector3 tmp;
	tmp.x=y*v1.z-v1.y*z;
	tmp.y=z*v1.x-v1.z*x;
	tmp.z=x*v1.y-v1.x*y;
	return tmp ;
}
Vector3& Vector3::operator^=(const Vector3&v1)
{
	static Vector3 tmp;
	tmp.x=y*v1.z-v1.y*z;
	tmp.y=z*v1.x-v1.z*x;
	tmp.z=x*v1.y-v1.x*y;
	*this=tmp;
	return *this ;
}

t_double Vector3::operator*(const Vector3&v1)const
{
	return x*v1.x+y*v1.y+z*v1.z;
}
Vector3 & Vector3::operator*=(t_double yy)
{
	x*=yy;
	y*=yy;
	z*=yy;
	return *this;
}
Vector3 & Vector3::operator*(t_double yy)const
{
	static Vector3 tmp;
	tmp.x=x*yy;
	tmp.y=y*yy;
	tmp.z=z*yy;
	return tmp;
}

Point3& Vector3::topoint ()const
{
	static Point3 p;
	p.x =x;
	p.y=y;
	p.z=z;
	return p;
}

t_double * Vector3::arrv()
{
return &x;	
	
}
