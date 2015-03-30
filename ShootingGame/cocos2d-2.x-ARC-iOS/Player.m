//
//  Player.m
//  ShootingGame
//
//  Created by KazukiKubo on 2013/02/20.
//  Copyright 2013年 __MyCompanyName__. All rights reserved.
//

#import "Player.h"


@implementation Player

-(id)init {
    if (self = [super init]) {
        self = [CCSprite spriteWithFile:@"Player.png"];
    }
    return self;
}

@end
