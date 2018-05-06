#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <string.h>

#include <unistd.h>

#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "treewalker.h"
#include <regex>
#include <string>
#include <fstream>

using namespace std;
using namespace cv;

//File type filter
bool imagefilter(const FileInfo& fi)
{
    static regex imagepattern(R"pat(.*?\.(jpg|jpeg|png)$)pat");
    if (fi.ftype == FileInfo::FType::FILE && regex_match(fi.basename,imagepattern))
        return true;
    else
        return false;
}

//FUnction finds contents below a given path and saves directories in vector of string
int find_filer(string location, vector<string> &outdir, vector<string> &outname)
{
    FileInfo fi;

    TreeWalker tw3(location,imagefilter);

    while(tw3.getNextEntry(fi)) {
        cout << "Found image file: " << fi.dirname << "/" << fi.basename << "\n";
        outdir.push_back(fi.dirname+"/"+fi.basename);
        outname.push_back(fi.basename);
    }

    cout << endl;

    return 0;
}

int main() {
    vector<string> location, outdir, outname;
    string input;
    cout << "Please enter path for folder: ";
    cin >> input;
    location.push_back(input);
    vector<string>::iterator it1;

    find_filer(location[0], outdir, outname);

    Mat picture;
    vector<vector<int>> data;
    int i;

    for (it1 = outdir.begin(); it1 != outdir.end(); ++it1) {
        cout << i << '\n';
        cout << outdir[i] << '\n';
        picture=imread(outdir[i]);
        if ( picture.empty() )
        {
            cout << "failed to load\n";
        }

        cvtColor(picture, picture, CV_BGR2GRAY);
        imshow("Picture",picture);
        vector<int> data_vec;
        char kategori;
        waitKey(1);
        kategori=outdir[i][location[0].length()+1];
        //cout << picture.ptr(1, 1);
        for (int x = 0; x < picture.rows; x++)
            for (int y = 0; y < picture.cols; y++)
                data_vec.push_back(picture.at<uchar>(x, y));
        /*for(auto x:data_vec){
            cout << x << ' ';
        }*/
        cout << "\n";
        data_vec.insert(data_vec.begin(),kategori-48);
        data.push_back(data_vec);

        i++;
        waitKey(1);
    }
/*    for(int i=0;i<data.size();i++){
        for(auto x:data[i]){
            cout << x << ' ';
        }
        cout << "\n";
    }*/
    cout << "\nMatrix size: \n" << data.size() << " : " << data[0].size() << "\nSaved as data.data, in same DIR as program";

    //Write to file
    ofstream myfile;
    myfile.open ("data.data");
    myfile <<  data.size() << " " << data[0].size() << "\n";
    for(int i=0;i<data.size();i++){
        for(auto x:data[i]){
            myfile << x << ' ';
        }
        myfile << "\n";
    }
    myfile.close();

    return 0;
}