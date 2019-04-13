#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(80, 80, 80);
    
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    
    
    camWidth = 640;
    camHeight = 480;
    
    gridSize = 5;
    referencePointSize = pow(gridSize,2);
    
    /*
     BASIS_SHAPE_TRIANGLE = 0,
     BASIS_SHAPE_SQUARE,
     BASIS_SHAPE_SQUARE_MINUS_HIGHEST,
     BASIS_SHAPE_PADE_FIRST,
     BASIS_SHAPE_PADE_SECOND
     */
    
    training_dataSet.init(2, 1, referencePointSize);
    polynomial_fit_bicubic.init(3, 2, 1,BASIS_SHAPE_TRIANGLE);

    initialiseData(1);
    
    switchDistribution = 0;
    
    
}

void ofApp::initialiseData(bool initMethode)
{
    
    DataType *my_p = training_dataSet.getInput();
    DataType *my_Z = training_dataSet.getOutput();
    
    ofLog()<<"training_dataSet.size() "<<training_dataSet.size();
    
    if(initMethode == 0){
        int cnt = 0;
        int cntZ = 0;
        float widthStep = camWidth/gridSize;
        float heightStep = camHeight/gridSize;
        
        for(int x=0; x<gridSize; x++){
            for(int y=0; y<gridSize; y++){
                my_p[cnt++] = x*widthStep;
                my_p[cnt++] = y*heightStep;
                my_Z[cntZ++] = y*4;
            }
        }
        
        my_p[0] = mouseX;
        my_p[1] = mouseY;
        my_Z[0] = 45;
    }
    

    
    if(initMethode == 1){
        for(int i=0; i<referencePointSize; i++){
            my_p[i*2] = ofRandom(camWidth);
            my_p[i*2+1] = ofRandom(camHeight);
            my_Z[i] = ofRandom(5,40);
        }
        
        
        my_p[0] = 0;
        my_p[1] = 0;
        my_Z[0] = 5;
        
        my_p[2] = camWidth;
        my_p[3] = 0;
        my_Z[1] = 5;
        
        my_p[4] = camWidth;
        my_p[5] = camHeight;
        my_Z[2] = 5;
        
        my_p[6] = 0;
        my_p[7] = camHeight;
        my_Z[3] = 5;
        
        my_p[8] = mouseX;
        my_p[9] = mouseY;
        my_Z[4] = 45;
        
    }
    polynomial_fit_bicubic.correlate(training_dataSet);
 
    
    ofLog()<<"initMethode "<<initMethode;
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    //ofLog()<<"-----------------------";
    
    ////////////////////////////////
    //  Draw value limit frame
    ////////////////////////////////
    ofSetColor(255);
    ofNoFill();
    ofDrawRectangle(0,0,camWidth,camHeight);
    
    
    
    ////////////////////////////////
    //  Draw newly evaluated points at other grid positions
    ////////////////////////////////
    pfitDataSet<DataType> pointTwo;
    pointTwo.init(2,1,1);
    
    int cnt = 0;
    int cntZ = 0;
    
    float widthStep = camWidth/9.;
    float heightStep = camHeight/9.;
    
    for(int x=0; x<9; x++){
        for(int y=0; y<9; y++){
            pointTwo.getInput()[0] = x*widthStep;
            pointTwo.getInput()[1] = y*heightStep;
            
            polynomial_fit_bicubic.evaluate(pointTwo);
            
            float resultDim = pointTwo.getOutput()[0];
            
            ofSetColor(0, 0, 255);
            ofDrawCircle(x*widthStep, y*heightStep, resultDim);
        }
    }
    
    ////////////////////////////////
    //  Draw newly evaluated point at mousePosition
    ////////////////////////////////
    pointTwo.getInput()[0] = mouseX;
    pointTwo.getInput()[1] = mouseY;
    polynomial_fit_bicubic.evaluate(pointTwo);
    float resultDim = pointTwo.getOutput()[0];
    ofSetColor(0, 0, 255);
    ofFill();
    ofDrawCircle(mouseX, mouseY, resultDim);
    
    
    ////////////////////////////////
    //  Draw original data points
    ////////////////////////////////
    ofEnableAlphaBlending();
    
    //my way
    ofSetColor(255, 0, 0,100);
    ofFill();
    for(int i=0; i<referencePointSize;i++){
        ofVec2f gridPoint(training_dataSet.getInput()[i*2],training_dataSet.getInput()[i*2+1]);
        float dim = training_dataSet.getOutput()[i];
        ofDrawCircle(gridPoint, dim);
    }
    
    
    //original way
    DataType *inPoints = training_dataSet.getInput();
    DataType *Y = training_dataSet.getOutput();
    
    ofNoFill();
    ofSetColor(150, 255, 150, 100);
    for (int i=0; i<referencePointSize; i++){
        ofDrawCircle(inPoints[i*2], inPoints[i*2+1], Y[i]);
    }
    
    ofDisableAlphaBlending();
    
    
    ofSetColor(255);
    ofDrawBitmapString("press space bar to get different initialization pattern", camWidth+50,20);
    
    ofDrawBitmapString("red circles are original reference points", camWidth+50,70);
    ofDrawBitmapString("blue circles are newly evaluated points", camWidth+50,90);
    ofDrawBitmapString("the diameter represents polyfit output", camWidth+50,110);
    
    ofDrawBitmapString("moving mouse evaluates mouse point", camWidth+50,160);
    ofDrawBitmapString("dragging mouse adds new reference point", camWidth+50,180);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key ==' '){
           switchDistribution = !switchDistribution;
        initialiseData(switchDistribution);
     

    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    mouseX = x;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    mouseX = x;
    switchDistribution = 0;
    initialiseData(switchDistribution);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

