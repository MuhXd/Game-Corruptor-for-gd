#include <Geode/Geode.hpp>
#include <random>
#include <thread>
#include <chrono>
#include <Geode/modify/CCScheduler.hpp> //<Geode/cocos/CCScheduler.h>

using namespace geode::prelude;
#include <random>
        // Random number generation utilities
auto randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
};

int randomInt(int min, int max){
    return min + rand() % (max - min + 1);
};

void randomizeNode(CCNode* node) {
	if (!node) return;
			// Randomize position
			if (!typeinfo_cast<CCSpriteBatchNode*>(node->getParent())) {
				if (node->getChildrenCount() <= 0 && !typeinfo_cast<CCMenu*>(node->getParent()->getParent()) || !typeinfo_cast<CCMenu*>(node) && typeinfo_cast<CCMenu*>(node->getParent()) && !typeinfo_cast<CCMenu*>(node->getParent()->getParent())) {
					node->setAnchorPoint({0,0});
					auto winSize = CCDirector::get()->getWinSize();
					float randomX = randomFloat(0, winSize.width); // Adjust range as needed
					float randomY = randomFloat(0, winSize.height);
					node->setPosition({randomX, randomY});
				}
			}
            // Randomize color (if node supports color)
            if (auto sprite = typeinfo_cast<CCSprite*>(node)) {
                int randomR = randomInt(0, 255);
                int randomG = randomInt(0, 255);
                int randomB = randomInt(0, 255);
                sprite->setColor(ccc3(randomR, randomG, randomB));
				int randomOpacity = randomInt(0, 255); // Adjust range for transparency level
            	sprite->setOpacity(randomOpacity);
            }
			 if (auto sprite = typeinfo_cast<CCMenu*>(node)) {
				auto children = node->getChildren();
				sprite->setPosition({0,0});
				 for (auto child : CCArrayExt<CCMenu*>(children) ) {
					child->setZOrder(randomInt(1,100));
				 }
			 }
            auto children = node->getChildren();
            for (auto child : CCArrayExt<CCNode*>(children) ) {
               if (child) {
				 randomizeNode(child); 
				}
			}
			
}
float framerate = 0;
class $modify(myCCScheduler, CCScheduler) {
	virtual void update(float dt) {
		CCScheduler::update(dt);
		framerate += dt;
		if (framerate < 0.5) {
			return;
		}
		framerate=0;
		auto scene = CCDirector::sharedDirector()->getRunningScene();
        if (!scene) return;

        auto children = scene->getChildren();
        for (auto children : CCArrayExt<CCNode*>(children)) {
            for (auto child : CCArrayExt<CCNode*>(children->getChildren())) {
                if (child && child->getParent()) {
                    randomizeNode(child);
                }
            }
        }
	}
};

