#include "model.hpp"

model::model(const std::string& file_name)
{
    std::ifstream file(file_name);
    if (!file.good())
    {
        throw std::runtime_error(std::string("can't open model file: ") + file_name);
    }
    std::string line;
    while (!file.eof()) {
        std::getline(file, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v.raw[i];
            _vertexes.push_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            std::array<int,3> f;
            int itrash, idx;
            iss >> trash; int i = 0;
            while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                f[i++] = idx - 1;
            }
            _faces.push_back(f);
        }
    }
}

int model::nverts()
{
    return (int)_vertexes.size();
}

int model::nfaces()
{
    return (int)_faces.size();
}

Vec3f& model::vert(int i)
{
    return _vertexes[i];
}

model::face_t& model::face(int idx)
{
    return _faces[idx];
}
