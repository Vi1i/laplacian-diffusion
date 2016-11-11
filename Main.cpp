/*******************************************************************************
 *
 ******************************************************************************/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sys/stat.h>

template <class T>
class LapDiff {
    public:
        LapDiff(std::string filename_in, std::string filename_out) {
            LapDiff::filename_out = filename_out;
            LapDiff::filename_in  = filename_in;
        }

        void Read() {
            LapDiff::ReadFile();
        }

        void Write(int steps) {
            switch(LapDiff::Dimension) {
                case 1:
                    LapDiff::Write1D(steps);
                    break;

                case 2:
                    LapDiff::Write2D(steps);
                    break;

                case 3:
                    LapDiff::Write3D(steps);
                    break;
            }
        }

        void Calculate(int steps) {
            switch(LapDiff::Dimension) {
                case 1:
                    LapDiff::Steps1D = (T **) malloc(steps * sizeof(T));
                    for(auto z = 0; z < steps; z++) {
                        LapDiff::Steps1D[z] = (T*) malloc(LapDiff::Dimensions[0]
                                * sizeof(T));
                    }

                    for(auto x = 0; x < LapDiff::Dimensions[0]; x++) {
                        auto value = LapDiff::Matrix1D[x];
                        LapDiff::Steps1D[0][x] = value;
                    }

                    LapDiff::LapDiff1D(steps);
                    break;

                case 2:
                    std::cout << "Feature Coming Soon!" << std::endl;
                    exit(EXIT_SUCCESS);
                    break;

                case 3:
                    std::cout << "Feature Coming Soon!" << std::endl;
                    exit(EXIT_SUCCESS);
                    break;
            }
        }

    private:
        size_t Dimension     = 0;
        T ***  Matrix3D;
        T **   Matrix2D;
        T *    Matrix1D;
        T **** Steps3D;
        T ***  Steps2D;
        T **   Steps1D;
        T      Dimensions[3] = {0, 0, 0};

        std::string filename_out;
        std::string filename_in;

        inline T CenteralDifference(T neg_m, T pos_m) {
            T result = 0;

            result = (neg_m + pos_m) / 2;

            return result;
        }

        inline void LapDiff3D(int steps) {
            size_t size_x = LapDiff::Dimensions[0];
            size_t size_y = LapDiff::Dimensions[1];
            size_t size_z = LapDiff::Dimensions[2];

            LapDiff::Steps3D = (T ****) calloc(steps * size_x * size_y * size_z,
                    sizeof(T));
            for(auto c_step = 0; c_step < steps; c_step++){
                for(auto x = 0; x < size_x; x++) {
                    for(auto y = 0; y < size_y; y++) {
                        for(auto z = 0; z < size_z; z++) {
                            auto px = LapDiff::Matrix3D[x][y][z];
                            auto neg_px = px;
                            auto pos_px = px;
                            auto neg_py = px;
                            auto pos_py = px;
                            auto neg_pz = px;
                            auto pos_pz = px;

                            if(x != 0) {
                                neg_px = LapDiff::Matrix3D[x - 1][y][z];
                            }
                            if(x = size_x - 1) {
                                pos_px = LapDiff::Matrix3D[x + 1][y][z];
                            }

                            if(y != 0) {
                                neg_py = LapDiff::Matrix3D[x][y - 1][z];
                            }
                            if(y = size_y - 1) {
                                pos_py = LapDiff::Matrix3D[x][y + 1][z];
                            }

                            if(z != 0) {
                                neg_pz = LapDiff::Matrix3D[x][y][z - 1];
                            }
                            if(z = size_z - 1) {
                                pos_px = LapDiff::Matrix3D[x][y][z + 1];
                            }

                            auto dx = CenteralDifference(neg_px, pos_px);
                            auto dy = CenteralDifference(neg_py, pos_py);
                            auto dz = CenteralDifference(neg_pz, pos_pz);

                            LapDiff::Steps3D[c_step][x][y][z] = dx + dy + dz;
                        }
                    }
                }
            }
        }

        inline void LapDiff2D(int steps) {
            size_t size_x = LapDiff::Dimensions[0];
            size_t size_y = LapDiff::Dimensions[1];

            LapDiff::Steps2D = (T ***) calloc(steps * size_x * size_y, sizeof(T));

            for(auto c_step = 0; c_step < steps; c_step++){
                for(auto x = 0; x < size_x; x++) {
                    for(auto y = 0; y < size_y; y++) {
                        auto px = LapDiff::Matrix2D[x][y];
                        auto neg_px = px;
                        auto pos_px = px;
                        auto neg_py = px;
                        auto pos_py = px;

                        if(x != 0) {
                            neg_px = LapDiff::Matrix2D[x - 1][y];
                        }
                        if(x = size_x - 1) {
                            pos_px = LapDiff::Matrix2D[x + 1][y];
                        }

                        if(y != 0) {
                            neg_py = LapDiff::Matrix2D[x][y - 1];
                        }
                        if(y = size_y - 1) {
                            pos_py = LapDiff::Matrix2D[x][y + 1];
                        }

                        auto dx = CenteralDifference(neg_px, pos_px);
                        auto dy = CenteralDifference(neg_py, pos_py);

                        LapDiff::Steps2D[c_step][x][y] = dx + dy;
                    }
                }
            }
        }

        inline void LapDiff1D(int steps) {
            size_t size_x = LapDiff::Dimensions[0];

            for(auto step = 1; step < steps; step++){
                for(auto x = 0; x < size_x; x++) {
                    auto px = LapDiff::Steps1D[step - 1][x];
                    auto neg_px = px;
                    auto pos_px = px;

                    if(x > 0) {
                        neg_px = LapDiff::Steps1D[step - 1][x - 1];
                    }
                    if(x < (size_x - 1)) {
                        pos_px = LapDiff::Steps1D[step - 1][x + 1];
                    }

                    auto dx = CenteralDifference(neg_px, pos_px);

                    LapDiff::Steps1D[step][x] = dx;
                }
            }
        }

        inline bool FileExists(const std::string& name) {
            struct stat buffer;   
            return (stat(name.c_str(), &buffer) == 0); 
        }

        inline void ReadFile() {
            if(!FileExists(filename_in)) {
                std::cerr << "File error on " << filename_in << std::endl;
                exit(EXIT_FAILURE);
            }
            std::ifstream in(filename_in.c_str());
            int dim = 0;
            std::string size_str = "";

            if(in.is_open()) {
                char c;
                while(in.get(c)) {
                    if(c == '\n') {
                        std::string::size_type sz;
                        LapDiff::Dimensions[dim] = stoi(size_str, &sz, 10);
                        size_str = "";
                        break;
                    }
                    if(c == 'x'){
                        std::string::size_type sz;
                        LapDiff::Dimensions[dim] = stoi(size_str, &sz, 10);
                        dim++;
                        size_str = "";
                        continue;
                    }
                    size_str += c;
                }
            }else{
                std::cerr << "File didn't open" << std::endl;
            }

            LapDiff::Dimension = dim + 1;
            if(dim >= 0) {
                std::cout << LapDiff::Dimensions[0];
            }
            if(dim >= 1) {
                std::cout << "x" << LapDiff::Dimensions[1];
            }
            if(dim >= 2) {
                std::cout << "x" << LapDiff::Dimensions[2];
            }
            std::cout << " " << LapDiff::Dimension <<"D" << std::endl;

            std::string value_str = "";
            char        c;
            auto        x         = 0;
            auto        y         = 0;
            auto        z         = 0;
            switch(LapDiff::Dimension) {
                case 1:
                    LapDiff::Matrix1D = (T *) malloc(LapDiff::Dimensions[0] *
                            sizeof(T));
                    value_str = "";

                    while(in.get(c)) {
                        if(c == '\n') {
                            std::string::size_type sz;
                            LapDiff::Matrix1D[x] = stoi(value_str, &sz, 10);
                            break;
                        }
                        if(c == ',') {
                            std::string::size_type sz;
                            LapDiff::Matrix1D[x] = stoi(value_str, &sz, 10);
                            value_str = "";
                            x++;
                            continue;
                        }
                        value_str += c;
                    }
                    break;

                case 2:
                    LapDiff::Matrix2D = (T **) malloc(LapDiff::Dimensions[0] *
                            LapDiff::Dimensions[1] * sizeof(T));
                    break;

                case 3:
                    LapDiff::Matrix3D = (T ***) malloc(LapDiff::Dimensions[0] *
                            LapDiff::Dimensions[1] * LapDiff::Dimensions[2] *
                            sizeof(T));
                    break;
            }

            in.close();
        }

        void Write1D(int steps) {
            std::ofstream out(LapDiff::filename_out.c_str());
            if(out.is_open()) {
                for(auto x = 0; x <= LapDiff::Dimensions[0]; x++) {
                    if(x == 0) {
                        //out << "Time(steps)";
                    }else{
                        out << ",var" << x;
                    }
                }
                out << std::endl;
                for(auto step = 0; step < steps; step++) {
                    if(step != 0) {
                        out << "--Step" << step << std::endl;
                    }
                    for(auto x = 0; x < LapDiff::Dimensions[0]; x++) {
                        if(x != 0) {
                            out << ",";
                        }
                        out << LapDiff::Steps1D[step][x];
                    }
                    out << std::endl;
                }
            }else{
                std::cerr << "Unable to open file" << std::endl;
            }

            out.close();
        }
        void Write2D(int steps) {
        }

        void Write3D(int steps) {
        }
};

int main(int argc, char * argv[]) {
    std::string filename_in;
    std::string filename_out;

    if(argc != 3) {
        std::cerr << argv[0] <<" [FILEIN] [FILEOUT]" << std::endl;
        exit(EXIT_FAILURE);
    }

    filename_in = argv[1];
    filename_out = argv[2];

    //LapDiff<float> * lap_diff = new LapDiff<float>(filename_in, filename_out);
    LapDiff<double> * lap_diff = new LapDiff<double>(filename_in, filename_out);
    //LapDiff<int> * lap_diff = new LapDiff<int>(filename_in, filename_out);

    int steps = 50;

    std::cout << "Reading in file..." << std::endl;
    lap_diff->Read();
    std::cout << "Calculating Matix..." << std::endl;
    lap_diff->Calculate(steps);
    std::cout << "Writing file..." << std::endl;
    lap_diff->Write(steps);

    exit(EXIT_SUCCESS);
}
