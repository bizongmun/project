#ifndef Cocos2DxFirstIosSample_CCParallaxNodeExtras_h
#define Cocos2DxFirstIosSample_CCParallaxNodeExtras_h

#include "cocos2d.h"

USING_NS_CC;

class ParallaxNodeExtras : public ParallaxNode {
    
    public :
    
	static ParallaxNodeExtras* create();
    
	void updatePosition();
    
} ;

#endif
