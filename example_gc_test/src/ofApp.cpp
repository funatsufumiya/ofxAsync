#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
}

//--------------------------------------------------------------
void ofApp::update(){
    int id = ofxAsync::run([](){
        ofSleepMillis(500);
    });

    thread_ids.push_back(id);

    // clear finished threads
    for(auto it = thread_ids.begin(); it != thread_ids.end(); ++it){
        if(!ofxAsync::exists(*it)){
            thread_ids.erase(it);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofDrawBitmapString("Press 'c' to stop all threads", 20, 20);
    ofDrawBitmapString("Thread count: " + ofToString(thread_ids.size()), 20, 40);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'c'){
        ofxAsync::stopAll();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
