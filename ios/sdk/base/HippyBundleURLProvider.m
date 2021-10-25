/*!
 * iOS SDK
 *
 * Tencent is pleased to support the open source community by making
 * Hippy available.
 *
 * Copyright (C) 2019 THL A29 Limited, a Tencent company.
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#import "HippyBundleURLProvider.h"

@interface HippyBundleURLProvider ()

@property (nonatomic, copy) NSString *localhostIP;
@property (nonatomic, copy) NSString *localhostPort;
@property (nonatomic, copy) NSString *debugPathUrl;

@end

@implementation HippyBundleURLProvider

#pragma mark - Life cycle

+ (instancetype)sharedInstance {
    static HippyBundleURLProvider *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[HippyBundleURLProvider alloc] init];
    });
    return sharedInstance;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        _localhostIP = @"172.20.10.3";
//        _localhostIP = @"localhost";
        _localhostPort = @"38989";
        _debugPathUrl = @"/index.bundle?platform=ios&dev=true&minify=false";
    }
    return self;
}

#pragma mark - Public

- (void)setLocalhostIP:(NSString *)localhostIP localhostPort:(NSString *)localhostPort {
    if (localhostIP) {
        _localhostIP = localhostIP;
    }
    if (localhostPort) {
        _localhostPort = localhostPort;
    }
}

- (void)setDebugPathUrl:(NSString *)debugPathUrl {
    if (debugPathUrl) {
        _debugPathUrl = debugPathUrl;
    }
}

- (NSString *)localhost {
    return [NSString stringWithFormat:@"%@:%@", _localhostIP, _localhostPort];
}

@end
