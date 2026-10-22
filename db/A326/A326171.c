/*
 * A326171  Let z be a sequence of distinct Gaussian integers such that z(1) =
 *          0, z(2) = 2+i (where i denotes the imaginary unit), for n > 1,
 *          z(n+1) the Gaussian integer with least norm at one knight move
 *          from z(n) (in case of a tie, choose the value such that
 *          Im(z(n+1)/z(n))>0); a(n) is the imaginary part o...
 * https://oeis.org/A326171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini
 * Formula:   not stated
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: simulate the knight walk on Gaussian integers.
   Start: z(1)=0, z(2)=2+i.
   For n>1, z(n+1) is the knight neighbour of z(n) that is
   unvisited, has minimum norm |z|^2, and in case of ties
   selects the neighbour with Im(z(n+1)/z(n))>0.
   a(n) = Im(z(n)). */

#include <stdlib.h>
#include <stdbool.h>

/* Global storage for real and imaginary parts of the walk. */
static seq_t *real_part = NULL;
static seq_t *imag_part = NULL;
static size_t capacity = 0;     /* allocated size (>= computed+1)      */
static size_t computed = 0;     /* highest index that has been computed */

/* Knight move offsets. */
static const int dx[8] = {  1,  2,  2,  1, -1, -2, -2, -1 };
static const int dy[8] = {  2,  1, -1, -2, -2, -1,  1,  2 };

/* Return true if point (r,i) appears among z(1)..z(upto). */
static bool is_visited(seq_t r, seq_t i, idx_t upto)
{
    for (idx_t k = 1; k <= upto; ++k) {
        if (real_part[k] == r && imag_part[k] == i)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* offset is 1; values below are irrelevant */

    /* Ensure allocated storage covers requested index. */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity > 0 ? capacity << 1 : 4;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        real_part = (seq_t *)realloc(real_part, new_cap * sizeof(seq_t));
        imag_part = (seq_t *)realloc(imag_part, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* Initialise first two terms if not yet done. */
    if (computed < 2) {
        real_part[1] = (seq_t)0;
        imag_part[1] = (seq_t)0;
        real_part[2] = (seq_t)2;
        imag_part[2] = (seq_t)1;
        computed = 2;
    }

    /* Generate missing terms up to n. */
    for (idx_t idx = computed + 1; idx <= n; ++idx) {
        seq_t curR = real_part[idx - 1];
        seq_t curI = imag_part[idx - 1];

        seq_t bestR = 0, bestI = 0;
        seq_t minNorm = 0;
        bool posBest = false;

        /* Find the first unvisited neighbour as initial candidate. */
        bool first = true;
        for (int m = 0; m < 8; ++m) {
            seq_t candR = curR + (seq_t)dx[m];
            seq_t candI = curI + (seq_t)dy[m];
            if (is_visited(candR, candI, idx - 1))
                continue;
            seq_t norm = candR * candR + candI * candI;
            if (first) {
                bestR = candR; bestI = candI; minNorm = norm;
                posBest = (candI * curR - candR * curI) > (seq_t)0;
                first = false;
            } else {
                bool posCand = (candI * curR - candR * curI) > (seq_t)0;
                if (norm < minNorm) {
                    bestR = candR; bestI = candI; minNorm = norm;
                    posBest = posCand;
                } else if (norm == minNorm && !posBest && posCand) {
                    bestR = candR; bestI = candI;
                    posBest = posCand;
                }
            }
        }

        /* Store the chosen neighbour. */
        real_part[idx] = bestR;
        imag_part[idx] = bestI;
    }

    computed = (size_t)n;
    return imag_part[n];
}
