#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <random>


struct Circle {
  double x;
  double y;
  double r;

  Circle() : x(0), y(0), r(0) {}
  Circle(double x, double y, double r) : x(x), y(y), r(r) {}

  [[nodiscard]] bool In(double x, double y) const {
    return (x - this->x) * (x - this->x) + (y - this->y) * (y - this->y) <= r * r;
  }
};

struct Rectangle {
  double ldx;
  double ldy;
  double rux;
  double ruy;

  Rectangle() : ldx(0), ldy(0), rux(0), ruy(0) {}

  double GetSquare() const {
    return (rux - ldx) * (ruy - ldy);
  }
};

std::istream& operator>>(std::istream& is, Circle& circle) {
  is >> circle.x >> circle.y >> circle.r;
  return is;
}


double CalculateSquare(const Circle& a, const Circle& b, const Circle& c, const Rectangle& bounds, size_t pointsCount) {
  size_t pointsIn = 0;

  std::random_device rand_dev;
  std::mt19937 generator(rand_dev());
  std::uniform_real_distribution<double> xDistribution(bounds.ldx, bounds.rux);
  std::uniform_real_distribution<double> yDistribution(bounds.ldy, bounds.ruy);

  for (size_t i = 0; i < pointsCount; ++i) {
    double x = xDistribution(generator);
    double y = yDistribution(generator);

    if (a.In(x, y) && b.In(x, y) && c.In(x, y)) {
      ++pointsIn;
    }
  }

  return static_cast<double>(pointsIn)/pointsCount * bounds.GetSquare();
}

template<typename T>
void PrintArrayIntoFile(std::ofstream& file, const std::vector<T>& array) {
  for (size_t i = 0; i < array.size(); ++i) {
    file << array[i];
    if (i < array.size() - 1) {
      file << ", ";
    }
  }
  file << '\n';
}


int main() {
  Circle a(1,1,1);
  Circle b(1.5,2, sqrt(5)/2);
  Circle c(2,1.5, sqrt(5)/2);

  Rectangle bounds;
  bounds.ldx = std::min({a.x - a.r, b.x - b.r, c.x - c.r});
  bounds.ldy = std::min({a.y - a.r, b.y - b.r, c.y - c.r});
  bounds.rux = std::max({a.x + a.r, b.x + b.r, c.x + c.r});
  bounds.ruy = std::max({a.y + a.r, b.y + b.r, c.y + c.r});

  double answer = 0.25 * M_PI + 1.25 * asin(0.8) - 1;

  std::vector<size_t> pointsCounts;
  std::vector<double> squares;
  std::vector<double> diffs;
  for(size_t pointsCount = 100; pointsCount <= 100000; pointsCount += 500){
    double square = CalculateSquare(a,b,c,bounds, pointsCount);
    squares.push_back(square);
    pointsCounts.push_back(pointsCount);
    diffs.push_back(std::abs(square - answer)/answer * 100);
  }

  std::ofstream outFile("../A1SmallBounds.txt");
  PrintArrayIntoFile(outFile, pointsCounts);
  PrintArrayIntoFile(outFile, squares);
  PrintArrayIntoFile(outFile, diffs);
  outFile.close();
}