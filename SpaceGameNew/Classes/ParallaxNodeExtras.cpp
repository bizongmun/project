#include "ParallaxNodeExtras.h"

class PointObject : public Ref
{
public:
	inline void setRation(Point ratio) {_ratio = ratio;}
	inline void setOffset(Point offset) {_offset = offset;}
	inline void setChild(Node *var) {_child = var;}
	inline Point getOffset() const {return _offset;}
	inline Node* getChild() const {return _child;}
private:
	Point _ratio;
	Point _offset;
	Node* _child;
};


ParallaxNodeExtras* ParallaxNodeExtras::create()
{
	ParallaxNodeExtras* node = new ParallaxNodeExtras();
	if(node) {
		node->autorelease();
	} else {
		delete node;
		node = 0;
	}
	return node;
}

void ParallaxNodeExtras::updatePosition()
{
	int safeOffset = 0;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	for(int i = 0; i < _children.size(); i++)
	{
		auto node = _children.at(i);
		if(convertToWorldSpace(node->getPosition()).x + node->getContentSize().width < safeOffset)
            for(int j = 0; j < _parallaxArray->num; j++)
            {
                auto po = (PointObject*)_parallaxArray->arr[j];
                if(po->getChild() == node)
                    
                    if (node->getContentSize().width<visibleSize.width)
                    {
                        po->setOffset(po->getOffset() +	Point(visibleSize.width + node->getContentSize().width,0));
                        
                    }else
                    {
                        po->setOffset(po->getOffset() +	Point(node->getContentSize().width*2,0));
                    }
                
                
            }
	}
}