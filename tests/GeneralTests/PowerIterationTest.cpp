//
// Created by TianKai Ma on 2023/12/19.
//

#include "../../includes/NLAMethods.h"
#include "../doctest.h"

TEST_CASE("PowerIteration::RevPowerIteration") {
    auto m = Matrix("[5 1 -2; 1 2 0; -2 0 -10]");
    auto I = Matrix::identity(m.rows);
    auto lambda = -10.263471;
    auto A = m - I * lambda;
    auto x = Vector("[1 1 1]");
    auto r = RevPowerIteration(PowerIterationInput{A, x, 1000});

    CHECK_LE(VectorNorm_Infinity(A * r.result), 1e-5);
}