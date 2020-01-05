#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

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

class MyByte{
    private:
        int a, b, c;
    public:
        MyByte(int a, int b, int c){
            this->a = a;
            this->b = b;
            this->c = c;
        }

        int getA() { return this->a; }
        int getB() { return this->b; }
        int getC() { return this->c; }
};

int computeSizeOfAmplitude(int amplitude){
    if(amplitude){
        if(amplitude == -1 || amplitude == 1){
            return 1;
        }
        else if(amplitude == -3 || amplitude == -2 || amplitude == 2 || amplitude == 3){
            return 2;
        }
        else if((amplitude >= -7 && amplitude <= -4) || (amplitude >= 4 && amplitude <= 7)){
            return 3;
        }
        else if((amplitude >= -15 && amplitude <= -8) || (amplitude >= 8 && amplitude <= 15)){
            return 4;
        }
        else if((amplitude >= -31 && amplitude <= -16) || (amplitude >= 16 && amplitude <= 31)){
            return 5;
        }
        else if((amplitude >= -63 && amplitude <= -32) || (amplitude >= 32 && amplitude <= 63)){
            return 6;
        }
        else if((amplitude >= -127 && amplitude <= -64) || (amplitude >= 64 && amplitude <= 127)){
            return 7;
        }
        else if((amplitude >= -255 && amplitude <= -128) || (amplitude >= 128 && amplitude <= 255)){
            return 8;
        }
        else if((amplitude >= -511 && amplitude <= -256) || (amplitude >= 256 && amplitude <= 511)){
            return 9;
        }
        else if((amplitude >= -1023 && amplitude <= -512) || (amplitude >= 512 && amplitude <= 1023)){
            return 10;
        }
        return 0;
    }
    else return 0;
}

vector<int> parcurgereMatrice(Block matrix) {
        vector<int> lista;
        lista.resize(64);
        int k = 0;
        int column = 0;
        int row = 0;
        lista[k] = matrix.get(row, column);
        do {
            k++;
            column++;
            lista[k] = matrix.get(row, column);
            do {
                k++;
                column--;
                row++;
                lista[k] = matrix.get(row, column);
            } while (column != 0);

            if (row == 7 )
                break;
            row++;
            k++;
            lista[k] = matrix.get(row, column);
            do {
                row--;
                column++;
                k++;
                lista[k] = matrix.get(row, column);
            } while (row != 0);
        } while (true);

        do {

            k++;
            column++;
            lista[k] = matrix.get(row, column);
            if (column == 7)
                break;
            do {
                k++;
                column++;
                row--;
                lista[k] = matrix.get(row, column);
            } while (column != 7);
            row++;
            k++;
            lista[k] = matrix.get(row, column);
            do {
                row++;
                column--;
                k++;
                lista[k] = matrix.get(row, column);
            } while (row != 7);
        } while (true);

        return lista;
    }

vector<vector<MyByte>> toBytes(vector<Block> Yblocks, vector<Block> Ublocks, vector<Block> Vblocks){
    vector<vector<MyByte>> res;
    for(int i=0; i<Yblocks.size(); i++){
        vector<int> ylst = parcurgereMatrice(Yblocks[i]);
        vector<int> ulst = parcurgereMatrice(Ublocks[i]);
        vector<int> vlst = parcurgereMatrice(Vblocks[i]);
        vector<MyByte> ybyte;
        vector<MyByte> ubyte;
        vector<MyByte> vbyte;
        int zeroCntY = 0;
        int zeroCntU = 0;
        int zeroCntV = 0;
        for(int j=0; j<ylst.size(); j++){
            if(j==0){
                MyByte bt = MyByte(-1, computeSizeOfAmplitude(ylst[j]), ylst[j]);
                ybyte.push_back(bt);
            }
            else{
                if(ylst[j]==0){
                    zeroCntY++;
                }
                else{
                    MyByte bt = MyByte(zeroCntY, computeSizeOfAmplitude(ylst[j]), ylst[j]);
                    zeroCntY = 0;
                    ybyte.push_back(bt);
                }
            }
        }
        ybyte.push_back(MyByte(0, 0, -1));
        for(int j=0; j<ulst.size(); j++){
            if(j==0){
                MyByte bt = MyByte(-1, computeSizeOfAmplitude(ulst[j]), ulst[j]);
                ubyte.push_back(bt);
            }
            else{
                if(ulst[j]==0){
                    zeroCntU++;
                }
                else{
                    MyByte bt = MyByte(zeroCntU, computeSizeOfAmplitude(ulst[j]), ulst[j]);
                    zeroCntU = 0;
                    ubyte.push_back(bt);
                }
            }
        }
        ubyte.push_back(MyByte(0, 0, -1));
        for(int j=0; j<vlst.size(); j++){
            if(j==0){
                MyByte bt = MyByte(-1, computeSizeOfAmplitude(vlst[j]), vlst[j]);
                vbyte.push_back(bt);
            }
            else{
                if(vlst[j]==0){
                    zeroCntV++;
                }
                else{
                    MyByte bt = MyByte(zeroCntV, computeSizeOfAmplitude(vlst[j]), vlst[j]);
                    zeroCntV = 0;
                    vbyte.push_back(bt);
                }
            }
        }
        vbyte.push_back(MyByte(0, 0, -1));
        res.push_back(ybyte);
        res.push_back(ubyte);
        res.push_back(vbyte);
    }
    return res;
}

void substract128(Block &Yblock, Block &Ublock, Block &Vblock){
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            Yblock.set(i, j, Yblock.get(i, j) - 128);
            Ublock.set(i, j, Ublock.get(i, j) - 128);
            Vblock.set(i, j, Vblock.get(i, j) - 128);
        }
    }
}

void add128(Block &Yblock, Block &Ublock, Block &Vblock){
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            Yblock.set(i, j, Yblock.get(i, j) + 128);
            Ublock.set(i, j, Ublock.get(i, j) + 128);
            Vblock.set(i, j, Vblock.get(i, j) + 128);
        }
    }
}

double alpha(int val){
    if( val>0 ){
        return 1;
    }
    else{
        return 1/sqrt(2);
    }
}

void fDCT(Block &Yblock, Block &Ublock, Block &Vblock){
    double constant = 1.00/4.00;

    Block Yaux(8), Uaux(8), Vaux(8);

    for(int u=0; u<8; u++){
        for(int v=0; v<8; v++){
            double sumY = 0.00;
            double sumU = 0.00;
            double sumV = 0.00;
            for(int x=0; x<8; x++){
                for(int y=0; y<8; y++){
                    double cosineU = cos(((2.00*x+1)*u*3.1415)/16.00);
                    double cosineV = cos(((2.00*y+1)*v*3.1415)/16.00);
                    sumY += Yblock.get(x, y)*cosineU*cosineV;
                    sumU += Ublock.get(x, y)*cosineU*cosineV;
                    sumV += Vblock.get(x, y)*cosineU*cosineV;
                }
            }
            Yaux.set(u, v, constant*alpha(u)*alpha(v)*sumY);
            Uaux.set(u, v, constant*alpha(u)*alpha(v)*sumU);
            Vaux.set(u, v, constant*alpha(u)*alpha(v)*sumV);
        }
    }

    for(int u=0; u<8; u++){
        for(int v=0; v<8; v++){
            Yblock.set(u, v, Yaux.get(u, v));
            Ublock.set(u, v, Uaux.get(u, v));
            Vblock.set(u, v, Vaux.get(u, v));
        }
    }
}

void iDCT(Block &Yblock, Block &Ublock, Block &Vblock){
    double constant=1.00/4.00;

    Block Yaux(8), Uaux(8), Vaux(8);

    for(int x=0; x<8; x++){
        for(int y=0; y<8; y++){
            double sumY = 0.00;
            double sumU = 0.00;
            double sumV = 0.00;
            for(int u=0; u<8; u++){
                for(int v=0; v<8; v++){
                    double cosineU = cos(((2.00*x+1)*u*3.1415)/16.00);
                    double cosineV = cos(((2.00*y+1)*v*3.1415)/16.00);
                    sumY += Yblock.get(u, v)*cosineU*cosineV*alpha(u)*alpha(v);
                    sumU += Ublock.get(u, v)*cosineU*cosineV*alpha(u)*alpha(v);
                    sumV += Vblock.get(u, v)*cosineU*cosineV*alpha(u)*alpha(v);
                }
            }
            Yaux.set(x, y, constant*sumY);
            Uaux.set(x, y, constant*sumU);
            Vaux.set(x, y, constant*sumV);
        }
    }

    for(int u=0; u<8; u++){
        for(int v=0; v<8; v++){
            Yblock.set(u, v, Yaux.get(u, v));
            Ublock.set(u, v, Uaux.get(u, v));
            Vblock.set(u, v, Vaux.get(u, v));
        }
    }
}

void quantization(Block &Yblock, Block &Ublock, Block &Vblock){
    double Q[8][8] = {
            {6, 4, 4, 6, 10, 16, 20, 24},
            {5, 5, 6, 8, 10, 23, 24, 22},
            {6, 5, 6, 10, 16, 23, 28, 22},
            {6, 7, 9, 12, 20, 35, 32, 25},
            {7, 9, 15, 22, 27, 44, 41, 31},
            {10, 14, 22, 26, 32, 42, 45, 37},
            {20, 26, 31, 35, 41, 48, 48, 40},
            {29, 37, 38, 39, 45, 40, 41, 40}
    };

    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            Yblock.set(i, j, (int)Yblock.get(i, j)/Q[i][j]);
            Ublock.set(i, j, (int)Ublock.get(i, j)/Q[i][j]);
            Vblock.set(i, j, (int)Vblock.get(i, j)/Q[i][j]);
        }
    }
}

void dequantization(Block &Yblock, Block &Ublock, Block &Vblock){
    double Q[8][8] = {
            {6, 4, 4, 6, 10, 16, 20, 24},
            {5, 5, 6, 8, 10, 23, 24, 22},
            {6, 5, 6, 10, 16, 23, 28, 22},
            {6, 7, 9, 12, 20, 35, 32, 25},
            {7, 9, 15, 22, 27, 44, 41, 31},
            {10, 14, 22, 26, 32, 42, 45, 37},
            {20, 26, 31, 35, 41, 48, 48, 40},
            {29, 37, 38, 39, 45, 40, 41, 40}
    };

    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            Yblock.set(i, j, (int)Yblock.get(i, j)*Q[i][j]);
            Ublock.set(i, j, (int)Ublock.get(i, j)*Q[i][j]);
            Vblock.set(i, j, (int)Vblock.get(i, j)*Q[i][j]);
        }
    }
}

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

Block decompress(Block b){
    Block c{8};
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            c.set(i,j,b.get(i/2, j/2));
        }
    }
    return c;
}

void encoder(vector<vector<float>> Y, vector<vector<float>> U, vector<vector<float>> V, vector<Block> &YBlocks, vector<Block> &UBlocks, vector<Block> &VBlocks){
    cout<<"\t - encoder lab1 running"<<endl;
    YBlocks=divideMatrixBy8(Y);
    vector<Block> aux=divideMatrixBy8(U);

    for(Block b:aux){
        UBlocks.push_back(compress(b));
    } 

    aux=divideMatrixBy8(V);

    for(Block b:aux){
        VBlocks.push_back(compress(b));
    }

    cout<<"\t - encoder lab2 running"<<endl;
    for(int i=0; i<UBlocks.size(); i++){
        UBlocks[i] = decompress(UBlocks[i]);
        VBlocks[i] = decompress(VBlocks[i]);
    }

    for(int i=0; i<YBlocks.size(); i++){
        substract128(YBlocks[i], UBlocks[i], VBlocks[i]);
    }
    for(int i=0; i<YBlocks.size(); i++){
        fDCT(YBlocks[i], UBlocks[i], VBlocks[i]);
    }
    for(int i=0; i<YBlocks.size(); i++){
        quantization(YBlocks[i], UBlocks[i], VBlocks[i]);
    }
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
    cout<<"\t - decoder lab2 running"<<endl;
    for(int i=0; i<Y.size(); i++){
        dequantization(Y[i],U[i],V[i]);
    }

    for(int i=0; i<Y.size(); i++){
        iDCT(Y[i],U[i],V[i]);
    }

    for(int i=0; i<Y.size(); i++){
        add128(Y[i],U[i],V[i]);
    }

    for(int i=0; i<U.size(); i++){
        U[i]=compress(U[i]);
        V[i]=compress(V[i]);
    }
    cout<<"\t - decoder lab1 running"<<endl;
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
    /*vector<vector<float>> Y, U, V;
    vector<Block> YBlocks, UBlocks, VBlocks;

    cout<<"Loading the file...."<<endl;
    loadMatrices(Y, U, V);
    
    cout<<"Encoding...."<<endl;
    encoder(Y, U, V, YBlocks, UBlocks, VBlocks);
    //vector<vector<MyByte>> res = toBytes(YBlocks, UBlocks, VBlocks);
    //for(int i=0; i<res[0].size(); i++){
    //    cout<< res[0][i].getA() << ' ' << res[0][i].getB() << ' ' << res[0][i].getC() << endl;
    //}
    cout<<"Decoding...."<<endl;
    decoder(YBlocks ,UBlocks ,VBlocks ,Y[0].size(), Y.size());
    cout<<"Done :)"<<endl;*/

    /*Block test{8};
    int Q[8][8] = {
            {150, 80, 20, 4, 1, 0, 0, 0},
            {92, 75, 18, 3, 1, 0, 0, 0},
            {26, 19, 13, 2, 1, 0, 0, 0},
            {3, 2, 2, 1, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0}
    };
    for(int i=0; i<8;i++){
        for(int j=0; j<8; j++){
            test.set(i, j, Q[i][j]);
        }
    }
    
    vector<int> lst = parcurgereMatrice(test);
    for(int i=0; i<lst.size(); i++){
        cout<<lst[i]<<' '; 
    }*/



    return 0;
}