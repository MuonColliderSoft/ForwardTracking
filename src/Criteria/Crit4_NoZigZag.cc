#include "Crit4_NoZigZag.h"
#include <cmath>
#include "TVector3.h"


using namespace FTrack;

Crit4_NoZigZag::Crit4_NoZigZag ( float prodMin , float prodMax ){
   
   
   _prodMin = prodMin;
   _prodMax = prodMax;
   
   _name = "Crit4_NoZigZag";
   _type = "4Hit";
   
   _saveValues = false;
   
}



bool Crit4_NoZigZag::areCompatible( Segment* parent , Segment* child )throw( BadSegmentLength ){
   
   
     
   
   
   
   if (( parent->getHits().size() == 3 )&&( child->getHits().size() == 3 )){ //this is a criterion for 3-segments
      


      IHit* a = parent->getHits()[0];
      IHit* b = parent->getHits()[1];
      IHit* c = parent->getHits()[2];
      IHit* d = child-> getHits()[2];
      
      float ax = a->getX();
      float ay = a->getY();
      float az = a->getZ();
      
      float bx = b->getX();
      float by = b->getY();
      float bz = b->getZ();
      
      float cx = c->getX();
      float cy = c->getY();
      float cz = c->getZ();
      
      float dx = d->getX();
      float dy = d->getY();
      float dz = d->getZ();
      

      
      TVector3 outerVec  (bx-ax , by-ay , bz-az );
      TVector3 middleVec (cx-bx , cy-by , cz-bz );
      TVector3 innerVec  (dx-cx , dy-cy , dz-cz );
      
  
      
      
      double angleXY1 = outerVec.Phi()-middleVec.Phi(); //the angle between 2-segments in the xy plane
      double angleXY2 = middleVec.Phi()-innerVec.Phi();
      
      angleXY1 -= 2*M_PI*floor( angleXY1 /2. /M_PI );    //to the range from 0 to 2pi 
      if (angleXY1 > M_PI) angleXY1 -= 2*M_PI;           //to the range from -pi to pi

      angleXY2 -= 2*M_PI*floor( angleXY2 /2. /M_PI );    //to the range from 0 to 2pi 
      if (angleXY2 > M_PI) angleXY2 -= 2*M_PI;           //to the range from -pi to pi
  
      
      // to grad
      angleXY1 *= 180./M_PI;
      angleXY2 *= 180./M_PI;

      float prod = angleXY1 * angleXY2; // if the direction of curvature stays the same, both anlges have the same sign-> and therefore the product is positive
      
      if (_saveValues) _map_name_value["Crit4_NoZigZag"] = prod;
      
      if ( prod < _prodMin ) return false;
      if ( prod > _prodMax ) return false;
      
         
      
   }
   else{
      
      std::string s = "Crit4_NoZigZag::This criterion needs 2 segments with 3 hits each, passed was a "
      +  intToString( parent->getHits().size() ) + " hit segment (parent) and a "
      +  intToString( child->getHits().size() ) + " hit segment (child).";
      
      
      throw BadSegmentLength( s );
      
      
   }
   
   return true;
   
   
   
}

