#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Block{
    private:
        vector<vector<float>> data;
        int n;
    public:
        Block(int n){
            this->n = n;
            for(int i=0; i<n; i++){
                vector<float> Aux;
                for(int j=0; j<n; j++){
                    Aux.push_back(-1);
                }
                data.push_back(Aux);
            }
        }

        float get(int i, int j){
            return data[i][j];
        }

        void set(int i, int j, float value){
            data[i][j] = value;
        }

        int getN(){
            return this->n;
        }
};

void loadMatrices(vector<vector<float>> &Y, vector<vector<float>> &U, vector<vector<float>> &V){
    ifstream in("nt-P3.ppm");
    string line;
    getline(in, line);
    getline(in, line);
    getline(in, line);
    
    int h = 0;
    int w = 0;
    istringstream ss(line);
    string word;
    ss>>word;
    w = stoi(word);
    ss>>word;
    h = stoi(word);

    getline(in, line);
    string p1, p2, p3;
    for(int i=0; i<h; i++){
        vector<float> A, B, C;
        for(int j=0; j<w; j++){
            A.push_back(-1);
            B.push_back(-1);
            C.push_back(-1);
        }
        Y.push_back(A);
        U.push_back(B);
        V.push_back(C);
    }

    for(int i=0; i<h; i++){
        string p1, p2, p3;
        for(int j=0; j<w; j++){
            getline(in, p1);
            getline(in, p2);
            getline(in, p3);
            Y[i][j] = 0.299*stof(p1) + 0.587*stof(p2) + 0.114*stof(p3);
            U[i][j] = 128 - 0.1687*stof(p1) - 0.3312*stof(p2) + 0.5*stof(p3);
            V[i][j] = 128 + 0.5*stof(p1) - 0.4186*stof(p2) - 0.0813*stof(p3);
        }
    }
}

vector<Block> divideMatrixBy8(vector<vector<float>> Matrix){
    vector<Block> blocks;

    for(int i=0; i<Matrix.size(); i+=8){
        for(int j=0; j<Matrix[i].size(); j+=8){
            Block b{8};
            for(int k=i; k<i+8; k++){
                for( int l=j; l<j+8; l++){
                    b.set(k-i, l-j, Matrix[k][l]);
                }
            }
            blocks.push_back(b);
        }
    }

    return blocks;
}

Block compress(Block b){
    Block c{4};
    for(int i=0; i<b.getN(); i+=2){
        for(int j=0; j<b.getN(); j+=2){
            float avg;
            float sum = 0;
            for(int k=i; k<i+2; k++){
                for(int l=j; l<j+2; l++){
                    sum += b.get(k, l);
                }
            }
            avg = sum/4;
            c.set(i/2, j/2, avg);
        }
    }
    return c;
}

void prettyBlockPrint(Block b){
    for(int i=0; i<b.getN(); i++){
        for(int j=0; j<b.getN(); j++){
            cout<<b.get(i, j)<<' ';
        }
        cout<<endl;
    }
}

void prettyMatrixPrint(vector<vector<float>> Matrix){
    for(int i=0; i<Matrix.size(); i++){
        for(int j=0; j<Matrix[i].size(); j++){
            cout<<Matrix[i][j]<<' ';
        }
        cout<<endl;
    }
    
}

void encoder(vector<vector<float>> Y, vector<vector<float>> U, vector<vector<float>> V, vector<Block> &YBlocks, vector<Block> &UBlocks, vector<Block> &VBlocks){
    YBlocks=divideMatrixBy8(Y);
    vector<Block> aux=divideMatrixBy8(U);

    for(Block b:aux){
        UBlocks.push_back(compress(b));
    } 

    aux=divideMatrixBy8(V);

    for(Block b:aux){
        VBlocks.push_back(compress(b));
    }

}


Block decompress(Block b){
    Block c{8};
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            c.set(i,j,b.get(i/2, j/2));
        }
    }
    return c;
}

vector<vector<float>> undivideMatrixBy8(vector<Block> blocks, int width, int height){
    vector<vector<float>> rez;
    
    for(int i=0; i<height; i++){
        vector<float> aux;
        for(int j=0; j<width; j++)
            aux.push_back(-1);
        rez.push_back(aux);
    }


    int nrBlock=0;
    for(int i=0; i<height; i+=8){
        for(int j=0; j<width; j+=8){
            for(int k=i;k<i+8;k++){
                for(int l=j;l<j+8;l++){
                    rez[k][l]=blocks[nrBlock].get(k-i,l-j);
                }
            }
            nrBlock++;
        }
    }

    return rez;

}


void writePPM(vector<vector<float>> Y, vector<vector<float>> U, vector<vector<float>> V){
    cout<<"Wrintng the PPM...."<<endl;
    ofstream out("result.ppm");

    out<<"P3";
    out<<endl;

    out<<"# CREATOR: GIMP PNM Filter Version 1.1";
    out<<endl;

    out<<Y[0].size()<<" "<<Y.size();
    out<<endl;

    out<<255;
    out<<endl;

    for(int i=0;i<Y.size();i++){
        for(int j=0;j<Y[i].size();j++){
            float R,G,B;
            R=Y[i][j]+1.402*(V[i][j]-128);
            G=Y[i][j]-0.344136*(U[i][j]-128)-0.714136*(V[i][j]-128);
            B=Y[i][j]+1.7790*(U[i][j]-128);

            if (R > 255) R = 255.0;
            if (G > 255) G = 255.0;
            if (B > 255) B = 255.0;

            if (R < 0) R = 0.0;
            if (G < 0) G = 0.0;
            if (B < 0) B = 0.0;

            out<<(int)R<<endl<<(int)G<<endl<<(int)B<<endl;

        }
    }

}


void decoder(vector<Block> Y, vector<Block> U, vector<Block> V, int width, int height){
    vector<Block> UDecompressed, VDecompressed;
    for(Block b:U){
        UDecompressed.push_back(decompress(b));
    }


    for(Block b:V){
        VDecompressed.push_back(decompress(b));
    }

    vector<vector<float>> YNoBlocks, UNoBlocks, VNoBlocks;

    YNoBlocks=undivideMatrixBy8(Y,width,height);
    UNoBlocks=undivideMatrixBy8(UDecompressed,width,height);
    VNoBlocks=undivideMatrixBy8(VDecompressed,width,height);

    writePPM(YNoBlocks, UNoBlocks, VNoBlocks);
    
}


int main(){
    vector<vector<float>> Y, U, V;
    vector<Block> YBlocks, UBlocks, VBlocks;

    loadMatrices(Y, U, V);
    
    cout<<"Encoding...."<<endl;
    encoder(Y, U, V, YBlocks, UBlocks, VBlocks);
    cout<<"Decoding...."<<endl;
    decoder(YBlocks ,UBlocks ,VBlocks ,Y[0].size(), Y.size());
    cout<<"Done :)"<<endl;

    return 0;
}