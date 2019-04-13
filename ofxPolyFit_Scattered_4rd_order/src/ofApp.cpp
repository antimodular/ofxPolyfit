#include "ofApp.h"

//float panList[] = {957.714,58.2857,86.4847,656,89.1429,1.6609,563.429,68.5714,-40.5663,320,61.7143,-87.9866,316.572,267.429,-55.5973,313.143,442.286,-38.9867,344,668.571,-25.7325,553.143,630.857,-9.0661,693.714,620.571,5.02,957.714,675.429,26.7333,786.286,538.286,13.9715,580.572,435.429,-7.6081};
//
//float tiltList[] ={957.714,58.2857,47.6683,656,89.1429,77.4733,563.429,68.5714,76.0887,320,61.7143,46.3373,316.572,267.429,42.0909,313.143,442.286,35.872,344,668.571,26.7358,553.143,630.857,27.708,693.714,620.571,28.0957,957.714,675.429,25.7637,786.286,538.286,32.4054,580.572,435.429,38.3046};

float panList[] = {947.429,675.429,25.9622,779.429,641.143,25.9622,724.571,634.286,10.9891,693.714,624,6.4489,553.143,624,-8.362,337.143,672,-26.5902,320,432,-39.1055,313.143,260.571,-55.6431,402.286,192,-52.2966,481.143,168,-42.901,501.714,126.857,-52.0612,409.143,188.571,-79.68,669.714,144,7.3376,947.429,65.1429,86.4765,964.571,157.714,66.2771,789.714,438.857,28.5187};

float tiltList[] ={947.429,675.429,25.3591,779.429,641.143,25.3591,724.571,634.286,25.4041,693.714,624,26.7808,553.143,624,27.1067,337.143,672,26.2189,320,432,33.6358,313.143,260.571,40.9674,402.286,192,49.0947,481.143,168,61.8644,501.714,126.857,64.1134,409.143,188.571,55.0575,669.714,144,69.6897,947.429,65.1429,49.3924,964.571,157.714,45.4049,789.714,438.857,35.2259};

ofVec2f resultArray[] = {ofVec2f(1,1),ofVec2f(2,2)};
//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(80, 80, 80);
    
    ofSetVerticalSync(true);
    // ofEnableSmoothing();
    
    
    camWidth = 1280; //640;
    camHeight = 720; //480;
    
    //    camWidth = 640;
    //    camHeight = 480;
    
    gridSize = 5;
    referencePointSize = (sizeof(panList)/sizeof(*panList)); //pow(gridSize,2);
    // referencePointSize = (sizeof(tiltList)/sizeof(*tiltList)); //pow(gridSize,2);
    
    
    /*
     BASIS_SHAPE_TRIANGLE = 0,
     BASIS_SHAPE_SQUARE,
     BASIS_SHAPE_SQUARE_MINUS_HIGHEST,
     BASIS_SHAPE_PADE_FIRST,
     BASIS_SHAPE_PADE_SECOND
     */
    
    
    initialiseData();
    
    switchDistribution = 0;
    
    
}

void ofApp::initialiseData()
{
    //2d x/y points input and 2d pan/tilt values output
    training_dataSet.init(2, 2, referencePointSize);
    
    //2d+2d makes a 4th order fit
    polynomial_fit_bicubic.init(4, 2, 2,BASIS_SHAPE_TRIANGLE);
    
    
    //prepare source data arrays
    DataType *my_p = training_dataSet.getInput();
    DataType *my_Z = training_dataSet.getOutput();
    
    ofLog()<<"training_dataSet.size() "<<training_dataSet.size();
    
    
    int panList_size = (sizeof(panList)/sizeof(*panList));
    ofLog()<<"panList size "<< panList_size ;
    int tiltList_size = (sizeof(tiltList)/sizeof(*tiltList));
    ofLog()<<"tiltList size "<<tiltList_size ;
    
    //fill source data arrays
    int cnt=0;
    for(int i=0; i<tiltList_size/3; i++){
        my_p[i*2] = (int)tiltList[i*3+0];
        my_p[i*2+1] = (int)tiltList[i*3+1];
        
        my_Z[i*2] = panList[i*3+2];
        my_Z[i*2+1] = tiltList[i*3+2];
        
        ofLog()<<"cnt "<<cnt<<" | "<<i*2<<" ->"<<my_p[i*2]<<" , "<<my_p[i*2+1]<<" , "<<my_Z[i*2]<<" , "<<my_Z[i*2+1];
        cnt++;
    }
    
    ofLog()<<"total cnt "<<cnt;
    
    
    polynomial_fit_bicubic.correlate(training_dataSet);
    
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
    int temp_size = 2;
    pointTwo.init(2,2,temp_size);
    
    int cnt = 0;
    int cntZ = 0;
    
    float widthStep = camWidth/10.;
    float heightStep = camHeight/10.;
    
    for(int x=0; x<10; x++){
        for(int y=0; y<10; y++){
            pointTwo.getInput()[0] = x*widthStep;
            pointTwo.getInput()[1] = y*heightStep;
            
            polynomial_fit_bicubic.evaluate(pointTwo);
            
            
            float resultDim_pan = pointTwo.getOutput()[0];
            float resultDim_tilt = pointTwo.getOutput()[1];
            
            ofSetColor(0, 0, 255);
            ofDrawCircle(x*widthStep, y*heightStep, resultDim_pan);
            ofSetColor(0);
            ofDrawCircle(x*widthStep, y*heightStep, resultDim_tilt/2);
        }
    }
    
    
    ////////////////////////////////
    //  Draw newly evaluated point at mousePosition
    ////////////////////////////////
    pointTwo.getInput()[0] = mouseX;
    pointTwo.getInput()[1] = mouseY;
    polynomial_fit_bicubic.evaluate(pointTwo);
    
    float resultDim_pan = pointTwo.getOutput()[0];
    float resultDim_tilt = pointTwo.getOutput()[1];
    
    ofSetColor(0, 0, 255);
    ofFill();
    ofDrawCircle(mouseX, mouseY, resultDim_pan);
    ofSetColor(0);
    ofDrawCircle(mouseX, mouseY, resultDim_tilt/2);
    
    ofSetColor(255);
    
    
    ofDrawBitmapStringHighlight("resultDim_tilt "+ofToString(resultDim_tilt), 20,50);
    ofDrawBitmapStringHighlight("resultDim_pan "+ofToString(resultDim_pan), 20,80);
  
    
    if(panArray.size() > 0){
        ofDrawBitmapStringHighlight("lookedUp_tilt "+ofToString(panArray[mouseY*camWidth+mouseX]), ofGetWidth()-300,50);
        ofDrawBitmapStringHighlight("lookedUp_pan "+ofToString(tiltArray[mouseY*camWidth+mouseX]), ofGetWidth()-300,80);
    }
    
    ofDrawBitmapStringHighlight(ofToString(mouseX)+" , "+ofToString(mouseY), mouseX,mouseY);

    
    
    
    ////////////////////////////////
    //  Draw original data points
    ////////////////////////////////
    ofEnableAlphaBlending();
    
    //my way
    
    for(int i=0; i<referencePointSize;i++){
        ofVec2f gridPoint(training_dataSet.getInput()[i*2],training_dataSet.getInput()[i*2+1]);
        
        float dim_pan = training_dataSet.getOutput()[i*2];
        ofSetColor(255, 0, 0,100);
        ofFill();
        ofDrawCircle(gridPoint, dim_pan);
        
        float dim_tilt = training_dataSet.getOutput()[i*2+1];
        ofSetColor(150, 255, 150);
        ofNoFill();
        ofDrawCircle(gridPoint, dim_tilt/2);
    }
    
    
    //original way / elliott's way
    /*
     DataType *my_p = training_dataSet.getInput();
     DataType *my_Z = training_dataSet.getOutput();
     
     ofNoFill();
     ofSetColor(150, 255, 150);
     
     int tiltList_size = (sizeof(tiltList)/sizeof(*tiltList));
     
     // ofLog()<<"referencePointSize "<<referencePointSize<<" , "<<tiltList_size;
     
     for (int i=0; i<tiltList_size/3; i++){
     ofCircle(my_p[i*2], my_p[i*2+1], my_Z[i*2+1]/2);
     
     //  ofLog()<<i<<" |"<<i*2<<" ->"<<my_p[i*2]<<" , "<<my_p[i*2+1]<<" , "<<my_Z[i*2]<<" , "<<my_Z[i*2+1];
     
     }
     */
    
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
        polynomial_fit_bicubic.correlate(training_dataSet);
    }
    
  
    
    if (key ==' '){
        initialiseData();
    }
    
    if(key == 'm'){
        makeLookUpTables();
    }
    
    if(key == 'o'){
        openLookUpTable();
    }
    
    
    if(key =='t'){
        pfitDataSet<DataType> pointTwo;
        int temp_size = 2;
        pointTwo.init(2,2,temp_size);
        
        float widthStep = 1;
        float heightStep = 1;
        
        panArray.clear();
        tiltArray.clear();
        
         for(int y=0; y<camHeight; y++){
             for(int x=0; x<camWidth; x++){
           
                pointTwo.getInput()[0] = x*widthStep;
                pointTwo.getInput()[1] = y*heightStep;
                
                polynomial_fit_bicubic.evaluate(pointTwo);
                
                float resultDim_tilt = pointTwo.getOutput()[0];
                float resultDim_pan = pointTwo.getOutput()[1];
                
                panArray.push_back(resultDim_pan);
                tiltArray.push_back(resultDim_tilt);
            }
        }
        
    }
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
void ofApp::windowResized(int w, int h){
    
}

void ofApp::openLookUpTable(){
    
    FILE * pFile;
    long lSize;
    uint16_t * buffer;
    size_t result;
    
    pFile = fopen (ofToDataPath("pan_lookuptable.bin").c_str(), "r" );
    
    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);
    
    // allocate memory to contain the whole file:
    buffer = (uint16_t*) malloc (sizeof(uint16_t)*lSize);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit();}
    
    // copy the file into the buffer:
    result = fread (buffer,1,lSize,pFile);
    if (result != lSize) {fputs ("Reading error",stderr); exit();}
    
    //cout << " found n elements " << result << endl;
    /* the whole file is now loaded in the memory buffer. */
    //cout << " lSize - " << lSize<< endl;
    cout << " found lookUpTable file ->      with fileSize = " << lSize <<  endl;
    
    memcpy(opened_panArray,buffer,sizeof(opened_panArray));
    
    // terminate
    fclose (pFile);
    free (buffer);
    //return 0;
    
    /*
     for(int i=0; i<camWidth*camHeight; i++){
     cout<<opened_panArray[i]<<",";
     }
     */
    
}
void ofApp::makeLookUpTables(){
    
    
    pfitDataSet<DataType> pointTwo;
    int temp_size = 2;
    pointTwo.init(2,2,temp_size);
    
    float widthStep = 1;
    float heightStep = 1;
    
    panArray.clear();
    tiltArray.clear();
    
    for(int y=0; y<camHeight; y++){
        for(int x=0; x<camWidth; x++){
            pointTwo.getInput()[0] = x*widthStep;
            pointTwo.getInput()[1] = y*heightStep;
            
            polynomial_fit_bicubic.evaluate(pointTwo);
            
            float resultDim_tilt = pointTwo.getOutput()[0];
            float resultDim_pan = pointTwo.getOutput()[1];
            
            panArray.push_back(resultDim_pan);
            tiltArray.push_back(resultDim_tilt);
        }
    }
    
    
    /*
     //ofVec2f resultArray[] = {ofVec2f(1,1),ofVec2f(2,2)};
     ofLog()<<"panArray() "<<panArray.size();
     for(int i=0; i<panArray.size();i++){
     cout<<panArray[i]<<",";
     }
     
     ofLog()<<"tiltArray() "<<tiltArray.size();
     for(int i=0; i<tiltArray.size();i++){
     cout<<tiltArray[i]<<",";
     }
     */
    
    //http://forum.openframeworks.cc/t/getting-to-know-ofbuffer-saving-complex-data-types/7240/2?u=stephanschulz
    
    string filename_pan = "pan_lookuptable.bin";
    ofFile panOutputFile;
    if (panOutputFile.open(filename_pan, ofFile::WriteOnly, true)) {
        
        for(int i = 0; i < panArray.size(); i++) {
            
            // write the raw data as if it were a stream of bytes
            panOutputFile.write((char*) &panArray[i], sizeof(float));
            
        }
    }
    panOutputFile.close();
    ofLog()<<"done making pan_lookuptable.bin";
    
    
    
    string filename_tilt = "tilt_lookuptable.bin";
    ofFile tiltOutputFile;
    if (tiltOutputFile.open(filename_tilt, ofFile::WriteOnly, true)) {
        
        for(int i = 0; i < tiltArray.size(); i++) {
            
            // write the raw data as if it were a stream of bytes
            tiltOutputFile.write((char*) &tiltArray[i], sizeof(float));
            
        }
    }
    tiltOutputFile.close();
    ofLog()<<"done making tilt_lookuptable.bin";
    
    
    
    
}



