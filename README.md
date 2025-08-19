# ofxAsync

- openFrameworks addon for 0.9.8 - 0.12.0
- Simple ofThread wrapper for easier multi-threading.
    - **Please that this is not real concurrent / asynchronous library like [asyncio](https://github.com/netcan/asyncio). Just thread wrapper.**
    - If you need real concurrent / asynchronous library, you should consider [ofxTaskRunner](https://github.com/funatsufumiya/ofxTaskRunner) instead.

## Comparison to [ofxTaskRunner](https://github.com/funatsufumiya/ofxTaskRunner)

`ofxTaskRunner` provides chainable methods which describes time-driven actions, while `ofxAsync` only provides simple thread wrapper.

`ofxTaskRunner` runs on single (main) thread, so you can use normal GL functions with asynchronous style. `ofxAsync` triggers real multi-thread, so it is useful hard time-taken tasks.

## Usage

The simplest example:

```cpp
ofxAsync::run([&](){
    // anything you want to make parallel

    ofSleepMillis(5000); // (ex) a task which takes time 
    ofLog() << "done! after 5 sec!";
});

ofLog() << "task started.";

// == logs ======
// [notice ] task started.
// [notice ] done! after 5 sec!
```

Another example with parameters:

```cpp
for(int i=0; i<20; ++i){
    ofxAsync::run([i](){ // <- capture variable i by copy
        ofSleepMillis(ofRandom(100,300));
        ofLog() << "thread No." << i;
    });
}

// == logs ======
// [notice ] thread No.9
// [notice ] thread No.13
// [notice ] thread No.7
// [notice ] thread No.11
// ...
// [notice ] thread No.15
// [notice ] thread No.3
```

( NOTE: about the **variable capture**, please read [this](https://en.cppreference.com/w/cpp/language/lambda) or other pages )


And more, `ofThread*` can be used as an argument:

```cpp
// Task cancelling example:

int thread_id = ofxAsync::run([&](ofThread* thread){
    for(int i=0; i<10; ++i){

        ofLog() << "processing " << (i+1) << " / 10";
        ofSleepMillis(1000);

        // cancell tasks when not running
        if(!thread->isThreadRunning()){
            ofLog() << "task cancelled";
            return;
        }
    }

    ofLog() << "done!";
});

ofLog() << "task started";
ofSleepMillis(3000);
ofxAsync::stop(thread_id); // Cancel task after 3 seconds

// == logs ======
// [notice ] task started
// [notice ] processing 1 / 10
// [notice ] processing 2 / 10
// [notice ] processing 3 / 10
// [notice ] task cancelled
```

## ofxAsyncInstance

This is instance-based version of ofxAsync.
Functionally same as ofxAsync, but you can use it as an instance.

This is useful when you want to manage multiple threads in a class.

NOTE: `ofxAsync::stopAll()` and `ofxAsync::waitForAll()` won't affect to ofxAsyncInstance. ofxAsync and ofxAsyncInstance are completely independent.

```cpp
// ofApp.h
#include "ofxAsyncInstance.h"

class ofApp : public ofBaseApp{
    // ....
    ofxAsyncInstance asyncInstance;
}

// ofApp.cpp
void ofApp::setup(){
    asyncInstance.run([](){
        ofLog() << "thread 1";
    });

    asyncInstance.run([](){
        ofLog() << "thread 2";
    });

    asyncInstance.waitForAll();
}
```
