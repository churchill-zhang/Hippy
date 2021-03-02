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

#import <Foundation/Foundation.h>

#import "HippyDefines.h"
@class HippyJSStackFrame;
/*
 * Defined in HippyUtils.m
 */
HIPPY_EXTERN BOOL HippyIsMainQueue(void);

/**
 * This is the main assert macro that you should use. Asserts should be compiled out
 * in production builds. You can customize the assert behaviour by setting a custom
 * assert handler through `HippySetAssertFunction`.
 */
#ifndef NS_BLOCK_ASSERTIONS
#define HippyAssert(condition, ...)                                                                                           \
    do {                                                                                                                      \
        if ((condition) == 0) {                                                                                               \
            _HippyAssertFormat(#condition, __FILE__, __LINE__, __func__, __VA_ARGS__);                                        \
            if (Hippy_NSASSERT) {                                                                                             \
                [[NSAssertionHandler currentHandler] handleFailureInFunction:@(__func__) file:@(__FILE__) lineNumber:__LINE__ \
                                                                 description:__VA_ARGS__];                                    \
            }                                                                                                                 \
        }                                                                                                                     \
    } while (false)
#else
#define HippyAssert(condition, ...) \
    do {                            \
    } while (false)
#endif
HIPPY_EXTERN void _HippyAssertFormat(const char *, const char *, int, const char *, NSString *, ...) NS_FORMAT_FUNCTION(5, 6);

/**
 * Report a fatal condition when executing. These calls will _NOT_ be compiled out
 * in production, and crash the app by default. You can customize the fatal behaviour
 * by setting a custom fatal handler through `HippySetFatalHandler`.
 */
HIPPY_EXTERN void HippyFatal(NSError *error);

HIPPY_EXTERN void MttHippyException(NSException *exception);

/**
 * The default error domain to be used for Hippy errors.
 */
HIPPY_EXTERN NSString *const HippyErrorDomain;

/**
 * JS Stack trace provided as part of an NSError's userInfo
 */
HIPPY_EXTERN NSString *const HippyJSStackTraceKey;

/**
 * Raw JS Stack trace string provided as part of an NSError's userInfo
 */
HIPPY_EXTERN NSString *const HippyJSRawStackTraceKey;

/**
 * Name of fatal exceptions generated by HippyFatal
 */
HIPPY_EXTERN NSString *const HippyFatalExceptionName;

/**
 * Module Name of fatal exceptions generated by HippyFatal
 */
HIPPY_EXTERN NSString *const HippyFatalModuleName;

/**
 * A block signature to be used for custom assertion handling.
 */
typedef void (^HippyAssertFunction)(NSString *condition, NSString *fileName, NSNumber *lineNumber, NSString *function, NSString *message);

typedef void (^HippyFatalHandler)(NSError *error);

typedef void (^MttHippyExceptionHandler)(NSException *e);

/**
 * Convenience macro for asserting that a parameter is non-nil/non-zero.
 */
#define HippyAssertParam(name) HippyAssert(name, @"'%s' is a required parameter", #name)

/**
 * Convenience macro for asserting that we're running on main queue.
 */
#define HippyAssertMainQueue() HippyAssert(HippyIsMainQueue(), @"This function must be called on the main thread")

/**
 * Convenience macro for asserting that we're running off the main queue.
 */
#define HippyAssertNotMainQueue() HippyAssert(!HippyIsMainQueue(), @"This function must not be called on the main thread")

/**
 * Deprecated, do not use
 */
#define HippyAssertMainThread() HippyAssertMainQueue()
#define HippyAssertNotMainThread() HippyAssertNotMainQueue()

/**
 * These methods get and set the current assert function called by the HippyAssert
 * macros. You can use these to replace the standard behavior with custom assert
 * functionality.
 */
HIPPY_EXTERN void HippySetAssertFunction(HippyAssertFunction assertFunction);
HIPPY_EXTERN HippyAssertFunction HippyGetAssertFunction(void);

/**
 * This appends additional code to the existing assert function, without
 * replacing the existing functionality. Useful if you just want to forward
 * assert info to an extra service without changing the default behavior.
 */
HIPPY_EXTERN void HippyAddAssertFunction(HippyAssertFunction assertFunction);

/**
 * This method temporarily overrides the assert function while performing the
 * specified block. This is useful for testing purposes (to detect if a given
 * function asserts something) or to suppress or override assertions temporarily.
 */
HIPPY_EXTERN void HippyPerformBlockWithAssertFunction(void (^block)(void), HippyAssertFunction assertFunction);

/**
 These methods get and set the current fatal handler called by the HippyFatal method.
 */
HIPPY_EXTERN void HippySetFatalHandler(HippyFatalHandler fatalHandler);
HIPPY_EXTERN HippyFatalHandler HippyGetFatalHandler(void);

HIPPY_EXTERN void MttHippySetExceptionHandler(MttHippyExceptionHandler exceptionhandler);
HIPPY_EXTERN MttHippyExceptionHandler MttHippyGetExceptionHandler(void);
/**
 * Get the current thread's name (or the current queue, if in debug mode)
 */
HIPPY_EXTERN NSString *HippyCurrentThreadName(void);

/**
 * Helper to get generate exception message from NSError
 */
// HIPPY_EXTERN NSString *HippyFormatError(NSString *message, NSArray<NSDictionary<NSString *, id> *> *stacktrace, NSUInteger maxMessageLength);
HIPPY_EXTERN NSString *HippyFormatError(NSString *message, NSArray<HippyJSStackFrame *> *stackTrace, NSUInteger maxMessageLength);

/**
 * Convenience macro to assert which thread is currently running (DEBUG mode only)
 */
#ifdef DEBUG

#define HippyAssertThread(thread, format...)                                                                                      \
    _Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"")                            \
        HippyAssert([(id)thread isKindOfClass:[NSString class]]   ? [HippyCurrentThreadName() isEqualToString:(NSString *)thread] \
                    : [(id)thread isKindOfClass:[NSThread class]] ? [NSThread currentThread] == (NSThread *)thread                \
                                                                  : dispatch_get_current_queue() == (dispatch_queue_t)thread,     \
            format);                                                                                                              \
    _Pragma("clang diagnostic pop")

#else

#define HippyAssertThread(thread, format...) \
    do {                                     \
    } while (0)

#endif
