/*
 * A306606  Number of ways to write n as w^2 + T(x)^2 + Pen(y)^2 + 2*Pen(z)^2,
 *          where w,x,y,z are integers with w > 0 and x >= 0, and T(m) =
 *          m*(m+1)/2 and Pen(m) = m*(3m-1)/2.
 * https://oeis.org/A306606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as w² + T(x)² + Pen(y)² +
 *            2·Pen(z)² where w > 0, x ≥ 0, y, z ∈ ℤ, T(m) = m(m+1)/2, Pen(m)
 *            = m(3m‑1)/2
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, pen_sq, pen_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/pen_sq.h"
#include "../utils/pen_val.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset safety */

    /* ----- Pre‑compute Pen(y)² values that can occur ----- */
    int yPosMax = -1;
    for (int m = 0; ; ++m) {
        seq_t v = pen_sq(m);
        if (v > n) break;
        yPosMax = m;
    }
    int yNegMax = 0;
    for (int m = 1; ; ++m) {
        seq_t v = pen_sq(-m);
        if (v > n) break;
        yNegMax = m;
    }

    size_t yCount = (size_t)(yPosMax + 1) + (size_t)(yNegMax);
    seq_t *yVals = (seq_t *)malloc(yCount * sizeof(seq_t));
    if (!yVals) return 0;
    size_t pos = 0;
    for (int m = -yNegMax; m <= yPosMax; ++m)
        yVals[pos++] = pen_sq(m);

    /* ----- Pre‑compute Pen(z)² values that can appear as 2·Pen(z)² ----- */
    int zPosMax = -1;
    for (int m = 0; ; ++m) {
        seq_t v = pen_sq(m);
        if (v * 2 > n) break;
        zPosMax = m;
    }
    int zNegMax = 0;
    for (int m = 1; ; ++m) {
        seq_t v = pen_sq(-m);
        if (v * 2 > n) break;
        zNegMax = m;
    }

    size_t zCount = (size_t)(zPosMax + 1) + (size_t)(zNegMax);
    seq_t *zVals = (seq_t *)malloc(zCount * sizeof(seq_t));
    if (!zVals) {
        free(yVals);
        return 0;
    }
    pos = 0;
    for (int m = -zNegMax; m <= zPosMax; ++m)
        zVals[pos++] = pen_sq(m);    /* store Pen(z)²; later we compare to half */

    /* ----- Main counting loop ----- */
    seq_t total = 0;
    seq_t sqrtN = floor_sqrt((seq_t)n);
    for (idx_t w = 1; w <= sqrtN; ++w) {
        seq_t w2 = (seq_t)w * (seq_t)w;
        seq_t remW = n - w2;

        seq_t sqrtW = floor_sqrt(remW);
        for (idx_t x = 0; x <= sqrtW; ++x) {
            seq_t T = (seq_t)x * (seq_t)(x + 1) / 2;
            seq_t T2 = T * T;
            if (T2 > remW) break;
            seq_t remWX = remW - T2;

            for (size_t i = 0; i < yCount; ++i) {
                seq_t ay = yVals[i];
                if (ay > remWX) continue;
                seq_t remYZ = remWX - ay;
                if (remYZ & 1) continue;          /* must be even */
                seq_t half = remYZ >> 1;          /* = Pen(z)² */

                for (size_t j = 0; j < zCount; ++j)
                    if (zVals[j] == half)
                        ++total;
            }
        }
    }

    free(yVals);
    free(zVals);
    return total;
}
