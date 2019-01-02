#include <iostream>
#include <armadillo>

void GenerateNoisySines(arma::cube& data,
                        arma::mat& labels,
                        const size_t points,
                        const size_t sequences,
                        const double noise = 0.3)
{
  arma::colvec x =  arma::linspace<arma::colvec>(0, points - 1, points) /
      points * 20.0;
  arma::colvec y1 = arma::sin(x + arma::as_scalar(arma::randu(1)) * 3.0);
  arma::colvec y2 = arma::sin(x / 2.0 + arma::as_scalar(arma::randu(1)) * 3.0);

  data = arma::zeros(1 /* single dimension */, sequences * 2, points);
  labels = arma::zeros(2 /* 2 classes */, sequences * 2);

  for (size_t seq = 0; seq < sequences; seq++)
  {
    arma::vec sequence = arma::randu(points) * noise + y1 +
        arma::as_scalar(arma::randu(1) - 0.5) * noise;
    for (size_t i = 0; i < points; ++i)
      data(0, seq, i) = sequence[i];

    labels(0, seq) = 1;

    sequence = arma::randu(points) * noise + y2 +
        arma::as_scalar(arma::randu(1) - 0.5) * noise;
    for (size_t i = 0; i < points; ++i)
      data(0, sequences + seq, i) = sequence[i];

    labels(1, sequences + seq) = 1;
  }
}

int main()
{
  const size_t rho = 10;
  
  for (int j = 0; j < 6; j++)
  {
    arma::cube input;
    arma::mat labelsTemp;
    GenerateNoisySines(input, labelsTemp, rho, 6);
  
    arma::cube labels = arma::zeros<arma::cube>(1, labelsTemp.n_cols, rho);
      for (size_t i = 0; i < labelsTemp.n_cols; ++i)
      {
        const int value = arma::as_scalar(arma::find(
            arma::max(labelsTemp.col(i)) == labelsTemp.col(i), 1)) + 1;
        labels.tube(0, i).fill(value);
      }
    std::cout << "input" << input << "labelsTemp" << labelsTemp << "labels" << labels << std::endl;
  }
  
  
  0;
}
