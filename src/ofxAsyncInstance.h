#pragma once

#include "ofMain.h"

/// \brief Instance version of ofxAsync
class ofxAsyncInstance {
private:
    class AsyncRunner: public ofThread {
    public:
        void setup(std::function<void()> _func){
            this->func = _func;
        }
        
        void threadedFunction(){
            this->func();
        }
        
        std::function<void()> func;
    };
    
    class AsyncRunnerWithArg: public ofThread {
    public:
        void setup(std::function<void(ofThread*)> _func){
            this->func = _func;
        }
        
        void threadedFunction(){
            this->func(this);
        }
        
        std::function<void(ofThread*)> func;
    };

    std::map<int, shared_ptr<ofThread> > runners;
    int thread_id_max;
    bool wait_for_all_when_exit;

    /// \brief Garbage collection for threads
    void gc();

public:
    ofxAsyncInstance();
    ~ofxAsyncInstance();

    int run(std::function<void()> func);
    int run(std::function<void(ofThread*)> func);
    /// \deprecated No need to call this method. Just exist for compatibility.
    /// garbage collection of threads is automatically done internally.
    void update();
    bool exists(int thread_id);
    bool isRunning(int thread_id);
    void stop(int thread_id, bool wait_until_stop=true);
    void stopAll(bool wait_until_stop=true);
    void waitFor(int thread_id);
    void waitForAll();
    void setExitAction(bool wait_for_all=true);
    /// NOTE: This method can return nullptr. Take care of it.
    /// FIXME: This is trade-off not using boost::optional<shared_ptr<ofThread>> as for compatibility issue.
    shared_ptr<ofThread> getThread(int thread_id);
};