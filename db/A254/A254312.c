/*
 * A254312  Rectangular array A read by upward antidiagonals in which the
 *          entry in row n and column k is defined by A(n,k) = (2^a(n)*(6*k -
 *          (3 - (-1)^a(n))*(1 - (-1)^n)/2) - 2^n + 4)/6, n,k >= 1, where
 *          {a(n)} is the Beatty sequence A117630 defined by a(n) =
 *          floor(n*log(3)/log(3/2)).
 * https://oeis.org/A254312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value obtained by reading the array A(n,k) by upward
 *            antidiagonals, where A(n,k) = (2^b(n)*(6*k - (3 - (-1)^b(n))*(1
 *            - (-1)^n)/2) - 2^n + 4)/6, with b(n) = floor(n*log(3)/log(3/2))
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/pow2.h"

static const double RATIO = log(3.0) / log(1.5);

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t diag = 2;   /* current diagonal (n + k) */
static idx_t cur_n = 1;  /* current n (row) within the diagonal */

static seq_t compute_cell(idx_t row, idx_t k)
{
    /* Beatty sequence value a(row) */
    uint64_t a_row = (uint64_t)(row * RATIO);
    /* Power of two values */
    seq_t pow2a = pow2((idx_t)a_row);
    seq_t pow2n = pow2(row);

    /* Compute B = (3 - (-1)^a(row))*(1 - (-1)^row)/2 */
    uint64_t B = 0;
    bool a_even = (a_row & 1ULL) == 0;
    bool n_even = (row & 1ULL) == 0;
    if (!n_even) {
        B = a_even ? 2 : 4;
    }

    seq_t s = (seq_t)(6 * k - B);   /* s = 6*k - B */
    /* Numerator: 2^a(row) * s - 2^row + 4 */
    seq_t term1 = pow2a * s;
    seq_t numerator = term1 - pow2n + 4;
    seq_t result = numerator / 6;
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;
    }

    /* Grow cache if necessary */
    while (cache_len < (size_t)n) {
        if (cache_len + 1 > cache_cap) {
            cache_cap = cache_cap ? 2 * cache_cap : 16;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache) {
                abort();  /* memory allocation failure */
            }
        }

        /* Current (row, col) pair */
        idx_t row = cur_n;
        idx_t k = diag - row;
        seq_t val = compute_cell(row, k);
        cache[cache_len++] = val;

        /* Advance to next pair in the antidiagonal ordering */
        if (cur_n == 1) {
            diag++;
            cur_n = diag - 1;
        } else {
            cur_n--;
        }
    }

    return cache[n - 1];
}
