/*
 * A265014  Triangle read by rows: T(n,k) = number of neighbors in
 *          n-dimensional lattice for generalized neighborhood given with
 *          parameter k.
 * https://oeis.org/A265014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = sum_{r=1..k} 2^r * binomial(n,r). Sequence a(n) is the
 *            triangle read by rows
 * Range:     u8 n <= 18, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static inline idx_t find_row(idx_t n)
{
    /* Binary search for the minimal r with r*(r+1)/2 >= n */
    idx_t low = 1;
    /* An upper bound on r is sqrt(2*n)+1, use double precision safely */
    idx_t high = (idx_t)(sqrt((long double)2.0L * (long double)n) + 2.0L);
    if (high < 1) high = 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Locate the cell (row, col) in the triangle that corresponds to the
       n-th element in row‑major order. */
    idx_t row = find_row(n);
    idx_t prev_sum = (row - 1) * row / 2;      /* Sum of lengths of previous rows */
    idx_t col = n - prev_sum;                  /* 1‑based column index within the row */

    /* Pre‑compute bitwidth and maximum value for the type seq_t. */
    const unsigned bitwidth = sizeof(seq_t) * 8;
    const seq_t seq_max = ~((seq_t)0);          /* All bits set */

    /* Compute the sum T(row, col) = Σ_{i=1..col} 2^i * binomial(row,i) */
    if (sizeof(seq_t) <= 8) {
        /* For small integer types use 128‑bit intermediates and guard against overflow. */
        __uint128_t sum128 = 0;
        __uint128_t pow2   = 1;                /* Will hold 2^i in 128‑bit */
        for (idx_t i = 1; i <= col; ++i) {
            if (i >= bitwidth) break;          /* Avoid UB for builtin types */
            pow2 <<= 1;                         /* pow2 = 2^i */
            seq_t bin = binomial(row, i);
            __uint128_t prod = (__uint128_t)bin * pow2;
            if (prod > (__uint128_t)seq_max) break;
            if (sum128 + prod > (__uint128_t)seq_max) break;
            sum128 += prod;
        }
        return (seq_t)sum128;
    } else {
        /* For arbitrary‑precision types simply use the type arithmetic. */
        seq_t sum = 0;
        seq_t pow2 = 1;                         /* 2^i in the big type */
        for (idx_t i = 1; i <= col; ++i) {
            pow2 <<= 1;                         /* pow2 = 2^i */
            seq_t bin = binomial(row, i);
            sum += bin * pow2;
        }
        return sum;
    }
}
