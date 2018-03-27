#include "stats.h"
/* returns the sums of all pixel values across all color channels.
* useful in computing the score of a rectangle
* PA3 function
* @param channel is one of r, g, or b
* @param ul is (x,y) of the upper left corner of the rectangle
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats:: getSum(char channel, pair<int,int> ul, pair<int,int> lr){
  //std::cout << "in getSum funtion" << '\n';
  vector< vector< long >> *sumCertainChannel=NULL;
  switch (channel) {
    case 'r': sumCertainChannel=&sumRed; break;
    case 'g': sumCertainChannel=&sumGreen; break;
    case 'b': sumCertainChannel=&sumBlue; break;
    default: return 0;
  }
  int x1=ul.first;
  int y1=ul.second;    //uper left value;
  int x2=lr.first;
  int y2=lr.second;    //lower right value;

  long sum=0;
  if(x1!=0 && y1!=0){
    sum=(*sumCertainChannel)[x2][y2]-(*sumCertainChannel)[x2][y1-1]-
             (*sumCertainChannel)[x1-1][y2]+(*sumCertainChannel)[x1-1][y1-1];
  }else if(x1!=0 && y1==0){
    sum=(*sumCertainChannel)[x2][y2]-(*sumCertainChannel)[x1-1][y2];
  }else if(x1==0 && y1!=0){
    sum=(*sumCertainChannel)[x2][y2]-(*sumCertainChannel)[x2][y1-1];
  }else{
    sum=(*sumCertainChannel)[x2][y2];
  }
  //the largest part -two parts+one overlaped part
  return sum;
}

/* returns the sums of squares of all pixel values across all color channels.
* useful in computing the score of a rectangle
* PA3 function
* @param channel is one of r, g, or b
* @param ul is (x,y) of the upper left corner of the rectangle
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats:: getSumSq(char channel, pair<int,int> ul, pair<int,int> lr){
  vector< vector< long >> *sumSqCertainChannel=NULL;
  switch (channel) {
    case 'r': sumSqCertainChannel=&sumsqRed; break;
    case 'g': sumSqCertainChannel=&sumsqGreen; break;
    case 'b': sumSqCertainChannel=&sumsqBlue; break;
    default: return 0;
  }
  int x1=ul.first;
  int y1=ul.second;    //uper left value;
  int x2=lr.first;
  int y2=lr.second;    //lower right value;

  long sum=0;
  if(x1!=0 && y1!=0){
    sum=(*sumSqCertainChannel)[x2][y2]-(*sumSqCertainChannel)[x2][y1-1]-
             (*sumSqCertainChannel)[x1-1][y2]+(*sumSqCertainChannel)[x1-1][y1-1];
  }else if(x1!=0 && y1==0){
    sum=(*sumSqCertainChannel)[x2][y2]-(*sumSqCertainChannel)[x1-1][y2];
  }else if(x1==0 && y1!=0){
    sum=(*sumSqCertainChannel)[x2][y2]-(*sumSqCertainChannel)[x2][y1-1];
  }else{
    sum=(*sumSqCertainChannel)[x2][y2];
  }
  //the largest part -two parts+one overlaped part
  return sum;
}

// initialize the private vectors so that, for each color,  entry
// (x,y) is the cumulative sum of the the color values from (0,0)
// to (x,y). Similarly, the sumSq vectors are the cumulative
// sum of squares from (0,0) to (x,y).
stats:: stats(PNG & im){
  unsigned int width=im.width();
  unsigned int height=im.height();
  resize(sumRed,width,height);
  resize(sumGreen,width,height);
  resize(sumBlue,width,height);
  resize(sumsqRed,width,height);
  resize(sumsqGreen,width,height);
  resize(sumsqBlue,width,height);
  for(unsigned int j=0;j<height;j++){ //y
    for(unsigned int i=0;i<width;i++){     //x
      long sumr=0;
      long sumg=0;
      long sumb=0;
      RGBAPixel * pixel=im.getPixel(i,j);
      unsigned long r=pixel->r;
      unsigned long g=pixel->g;
      unsigned long b=pixel->b;
      if(i!=0 && j!=0){
        sumr=sumRed[i-1][j]+sumRed[i][j-1]-sumRed[i-1][j-1]+r;    //minus overlaped
        sumg=sumGreen[i-1][j]+sumGreen[i][j-1]-sumGreen[i-1][j-1]+g;
        sumb=sumBlue[i-1][j]+sumBlue[i][j-1]-sumBlue[i-1][j-1]+b;
      }else if(i==0 && j!=0){
        sumr=sumRed[i][j-1]+r;
        sumg=sumGreen[i][j-1]+g;
        sumb=sumBlue[i][j-1]+b;
      }else if(i!=0 && j==0){
        sumr=sumRed[i-1][j]+r;
        sumg=sumGreen[i-1][j]+g;
        sumb=sumBlue[i-1][j]+b;
      }else{
        sumr=r;
        sumg=g;
        sumb=b;
      }
      sumRed[i][j]=sumr;
      sumGreen[i][j]=sumg;
      sumBlue[i][j]=sumb;
      //get
      long sumsqr=0;
      long sumsqg=0;
      long sumsqb=0;
      if(i!=0 && j!=0){
        sumsqr=sumsqRed[i-1][j]+sumsqRed[i][j-1]-sumsqRed[i-1][j-1]+r*r;    //minus overlaped
        sumsqg=sumsqGreen[i-1][j]+sumsqGreen[i][j-1]-sumsqGreen[i-1][j-1]+g*g;
        sumsqb=sumsqBlue[i-1][j]+sumsqBlue[i][j-1]-sumsqBlue[i-1][j-1]+b*b;
      }else if(i==0 && j!=0){
        sumsqr=sumsqRed[i][j-1]+r*r;
        sumsqg=sumsqGreen[i][j-1]+g*g;
        sumsqb=sumsqBlue[i][j-1]+b*b;
      }else if(i!=0 && j==0){
        sumsqr=sumsqRed[i-1][j]+r*r;
        sumsqg=sumsqGreen[i-1][j]+g*g;
        sumsqb=sumsqBlue[i-1][j]+b*b;
      }else{
        sumsqr=r*r;
        sumsqg=g*g;
        sumsqb=b*b;
      }
      sumsqRed[i][j]=sumsqr;
      sumsqGreen[i][j]=sumsqg;
      sumsqBlue[i][j]=sumsqb;
    }
  }
}
void stats::resize(vector< vector< long >> &sumRed,int width,int height){
  sumRed.resize(width);
  for(int a=0;a<width;a++){
    sumRed[a].resize(height,0);
  }
}
// given a rectangle, compute its sum of squared deviations from
      // mean, over all color channels. Will be used to make split when
// building tree.
/* @param ul is (x,y) of the upper left corner of the rectangle
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats:: getScore(pair<int,int> ul, pair<int,int> lr){
    //std::cout << "in getscore function" << '\n';
    long sumRDeviations=getSumSq('r',ul,lr)-(getSum('r',ul,lr)*getSum('r',ul,lr)/rectArea(ul,lr));
    long sumGDeviations=getSumSq('g',ul,lr)-(getSum('g',ul,lr)*getSum('g',ul,lr)/rectArea(ul,lr));
    long sumBDeviations=getSumSq('b',ul,lr)-(getSum('b',ul,lr)*getSum('b',ul,lr)/rectArea(ul,lr));
    return sumRDeviations+sumGDeviations+sumBDeviations;
}

// given a rectangle, return the average color value over the
// rectangle as a pixel.
/* Each color component of the pixel is the average value of that
* component over the rectangle.
* @param ul is (x,y) of the upper left corner of the rectangle
* @param lr is (x,y) of the lower right corner of the rectangle */
RGBAPixel stats:: getAvg(pair<int,int> ul, pair<int,int> lr){
    int aver=getSum('r',ul,lr)/rectArea(ul,lr);
    int aveg=getSum('g',ul,lr)/rectArea(ul,lr);
    int aveb=getSum('b',ul,lr)/rectArea(ul,lr);
    return RGBAPixel(aver,aveg,aveb);
}

// given a rectangle, return the number of pixels in the rectangle
/* @param ul is (x,y) of the upper left corner of the rectangle
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats:: rectArea(pair<int,int> ul, pair<int,int> lr){
    return (lr.second-ul.second+1)*(lr.first-ul.first+1);
}
