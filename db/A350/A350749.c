/*
 * A350749  Triangle read by rows: T(n,k) is the number of oriented graphs on
 *          n labeled nodes with k arcs, n >= 0, k = 0..n*(n-1)/2.
 * https://oeis.org/A350749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where T(r,k) = 2^k * binomial(r*(r-1)/2, k) and
 *            r,k are obtained from the index n by reading the triangle
 *            row‑wise
 * Range:     u8 n <= 17, u16 n <= 30, u32 n <= 73, u64 n <= 152,
 *            u128 n <= 419, big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t r = 0;
    idx_t cum = 0;
    while (1) {
        /* length of row r: 1 + number of unordered pairs among r nodes */
        idx_t row_len = 1 + (r * (r - 1)) / 2;
        if (cum + row_len > n)
            break;
        cum += row_len;
        r++;
    }
    idx_t k = n - cum;
    idx_t pairs = r * (r - 1) / 2;

    seq_t coeff   = binom(pairs, k);  /* binomial coefficient */
    seq_t two_pow = pow2(k);           /* 2^k */
    return coeff * two_pow;
}
