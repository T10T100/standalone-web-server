#ifndef GC_DIMENSIONS
#define GC_DIMENSIONS


template <typename Range>
    class Point {
        public :
            union {
              Range x;
              Range w;
            };
            union {
              Range y;
              Range h;
            };         
    };
    
template <typename Range>
    class Box {
        public :
              Range x;
              Range y;
              Range w;
              Range h;
    };



template <typename Range>
class Dimension {
    private :
         
    public :
        Range x, y, w, h, dx, dy;
        Dimension ()
        {
            this->x = 0;
            this->y = 0;
            this->w = 0;
            this->h = 0;
        }
        Dimension (int x, int y)
        {
            this->x  = x;
            this->y = y;
            this->w = 0;
            this->h = 0;
        }
        Dimension (int x, int y, int w, int h = 0)
        {
            this->x  = x;
            this->y = y;
            this->w = w;
            this->h = h;
        }
        Dimension (Dimension<Range> &d)
        {
            this->x  = d.x;
            this->y = d.y;
            this->w = d.w;
            this->h = d.h;
        }
        Dimension<Range> operator = (Dimension<Range> &d)
        {
            this->x  = d.x;
            this->y = d.y;
            this->w = d.w;
            this->h = d.h;
            return *this;
        }
        void operator () (int x, int y, int w = 0, int h = 0)
        {
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
        }
        template <typename D>
        Dimension<Range> operator = (D &b)
        {
            this->x  = b.x;
            this->y = b.y;
            this->w = b.w;
            this->h = b.h;
            return *this;
        }
        
        
        void setSize (int x, int y, int w = 0, int h = 0)
        {
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
        }
        void setSize (Dimension d)
        {
            this->x = d.x;
            this->y = d.y;
            this->w = d.w;
            this->h = d.h;
        }
        template <typename Plant>
        void setSize (Plant d)
        {
            this->x = d.x;
            this->y = d.y;
            this->w = d.w;
            this->h = d.h;
        }
        void setDefaultSize ()
        {
            this->x = 0;
            this->y = 0;
            this->w = 100;
            this->h = 100;
        }
        void setX (int x)
        {
            this->x = x;
        }
        void setY (int x)
        {
            this->y = x;
        }
        void setW (int x)
        {
            this->w = x;
        }
        void setH (int x)
        {
            this->h = x;
        }
        void setOrigins (Point<Range> p)
        {
            this->x = p.x - this->w / 2;
            this->y = p.y - this->h / 2;
        }
        
        void setTopLeftCorner (Point<Range> p)
        {
            x = p.x;
            y = p.y;
        }
        void setTopRightCorner (Point<Range> p)
        {
            x = p.x - w;
            y = p.y;
        }
        void setBottomLeftCorner (Point<Range> p)
        {
            x = p.x;
            y = p.y - h;
        }
        void setBottomRightCorner (Point<Range> p)
        {
            x = p.x - w;
            y = p.y - h;
        }
        
        void setTopCenter (Point<Range> p)
        {
            x = p.x - w / 2;
            y = p.y;
        }
        void setBottomCenter (Point<Range> p)
        {
            x = p.x - w / 2;
            y = p.y - h;
        }
        void setLeftCenter (Point<Range> p)
        {
            x = p.x;
            y = p.y - h / 2;
        }
        void setRightCenter (Point<Range> p)
        {
            x = p.x - w;
            y = p.y - h / 2;
        }
        
        
        
        int getX ()
        {
            return this->x;
        }
        int getY ()
        {
            return this->y;
        }
        int getWidth ()
        {
            return this->w;
        }
        int getHeight ()
        {
            return this->h;
        }
        Point<Range> getPlacement ()
        {
            Point<Range> p;
            p.x = this->x;
            p.y = this->y;
            return p;
        }
        Point<Range> getSize ()
        {
            Point<Range> p;
            p.w = this->w;
            p.h = this->h;
            return p;
        }
        
        Point<Range> getTopLeftCorner ()
        {
            Point<Range> p = {x, y};
            return p;
        }
        Point<Range> getTopRightCorner ()
        {
            Point<Range> p = {x + w, y};
            return p;
        }
        Point<Range> getBottomLeftCorner ()
        {
            Point<Range> p = {x, y + h};
            return p;
        }
        Point<Range> getBottomRightCorner ()
        {
            Point<Range> p = {x + w, y + h};
            return p;
        }
        
        Point<Range> getTopCenter ()
        {
            Point<Range> p = {x + w / 2, y};
            return p;
        }
        Point<Range> getBottomCenter ()
        {
            Point<Range> p = {x + w / 2, y + h};
            return p;
        }
        Point<Range> getLeftCenter ()
        {
            Point<Range> p = {x, y + h / 2};
            return p;
        }
        Point<Range> getRightCenter ()
        {
            Point<Range> p = {x + w, y + h / 2};
            return p;
        }
        
        Point<Range> getOrigins ()
        {
            Point<Range> p;
            p.x = this->x + this->w / 2;
            p.y = this->y + this->h / 2;
            return p;
        }
        Box<Range> getBox ()
        {
            Box<Range> box = {
                    this->x,
                    this->y,
                    this->w,
                    this->h
            };
            return box;
        }
        Dimension<Range> getDimension ()
        {
            Dimension<Range> d(x, y, w, h);
            return d;
        }
        
        template <typename D>
        bool trunc (D &box)
        {   	
            if (box.x > this->w) {
                  box.x = this->w;
            }
            if (box.x + box.w > this->w) {
                  box.w = this->w - box.x;
            }
          if (box.y > this->h) {
                  box.y = this->h;
            }
            if (box.y + box.h > this->h) {
                  box.h = this->h - box.y;
            }
            
            box.x += this->x;
            box.y += this->y;
            return 0;
        }
        
        bool testPoint (int x, int y)
        {
            int x0 = x - this->x;
            if (x0 < 0) {
                return false;
            }
            if (x0 > this->w) {
                return false;
            }
            int y0 = y - this->y;
            if (y0 < 0) {
                return false;
            }
            if (y0 > this->h) {
                return false;
            }
            return true;
        }
        
        bool testPoint (Point<Range> p) 
        {
            return testPoint(p.x, p.y);
        }
        
        template <typename D>
        D &normalize (D &d)
        {
            d.x  = d.x - x;
            if (d.x < 0 || d.x > w) {
                d.x = x;
            }
            if (d.x + d.w > w) {
                d.w = w - x;
            }
            d.y  = d.y - y;
            if (d.y < 0 || d.y > h) {
                d.y = y;
            }
            if (d.y + d.h > h) {
                d.h = h - y;
            }   
            return d;
        }
        
        Point<Range> normalize (Range x0, Range y0)
        {
            x0 = x0 - x;
            if (x0 < 0) {
                x0 = x;
            }
            y0 = y0 - x;
            if (y0 < 0) {
                y0 = x;
            }
            Point<Range> p = {x0, y0};
            return p;
        }
        
        Point<Range> normalize (Point<Range> p) 
        {
            return normalize(p.x, p.y);
        }
        
};



#endif

/*End of file*/

