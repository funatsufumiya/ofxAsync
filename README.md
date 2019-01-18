# ofxAsync

- openFrameworks addon for 0.9.8 - 0.10.1
- Simple ofThread wrapper for easier multi-threading.


## Usage

```cpp
ofxAsync::run([&](){
    // anything you want to make parallel

    ofSleepMillis(5000); // (ex) a task which takes time 
    ofLog() << "done!";
});
```

You can also take `ofThread*` as an arg:

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

// LOG:
[notice ] task started
[notice ] processing 1 / 10
[notice ] processing 2 / 10
[notice ] processing 3 / 10
[notice ] task cancelled
```
