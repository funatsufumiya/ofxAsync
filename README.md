# ofxAsync

- openFrameworks addon for 0.9.8 - 0.12.0
- Simple ofThread wrapper for easier multi-threading.


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