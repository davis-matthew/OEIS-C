/*
 * A361636  Diagonal of the rational function 1/(1 - v*w*x*y*z * (1 + 1/v +
 *          1/w + 1/x + 1/y + 1/z)).
 * https://oeis.org/A361636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} (n+k)!/(k!^5 * (n-4*k)!)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 45,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Diagonal of 1/(1 - v*w*x*y*z*(1+1/v + 1/w + 1/x + 1/y + 1/z)), offset 0 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"          /* provides seq_t gcd(seq_t a, seq_t b) */

/* Multiply cur by the next term’s numerators and divide by the denominators
   using GCD reductions to avoid overflow.  Works for any seq_t implementation
   that is unsigned (uint8_t …, __int128, arbitrary precision). */
static inline seq_t next_term(seq_t cur, idx_t n, idx_t k)
{
    /* Numerators of the recurrence:
       (n+k+1)*(n-4k)*(n-4k-1)*(n-4k-2)*(n-4k-3) */
    seq_t numer[5];
    numer[0] = (seq_t)(n + k + 1);
    numer[1] = (seq_t)(n - 4 * k);
    numer[2] = (seq_t)(n - 4 * k - 1);
    numer[3] = (seq_t)(n - 4 * k - 2);
    numer[4] = (seq_t)(n - 4 * k - 3);

    /* Denominators: five copies of (k+1) */
    seq_t denom[5];
    for (int i = 0; i < 5; ++i)
        denom[i] = (seq_t)(k + 1);

    /* Cancel common factors between numerators and denominators. */
    for (int i = 0; i < 5; ++i) {
        seq_t d = denom[i];
        if (d == (seq_t)1) continue;
        for (int j = 0; j < 5; ++j) {
            if (numer[j] == (seq_t)1) continue;
            seq_t g = gcd(numer[j], d);
            if (g > (seq_t)1) {
                numer[j] /= g;
                d      /= g;
                if (d == (seq_t)1) break;
            }
        }
        denom[i] = d;
    }

    /* Remaining denominators must divide cur exactly. */
    seq_t res = cur;
    for (int i = 0; i < 5; ++i) {
        seq_t d = denom[i];
        if (d > (seq_t)1)
            res /= d;
    }

    /* Multiply by the remaining numerators. */
    for (int i = 0; i < 5; ++i)
        res *= numer[i];

    return res;
}

/* Return a(n) for any n >= 0.  The implementation handles arbitrary
   seq_t sizes and avoids overflow for uint64_t by canceling factors
   before multiplication. */
seq_t a(idx_t n)
{
    idx_t max_k = n / 4;
    seq_t sum   = 0;
    seq_t term  = 1;                  /* term for k = 0 */

    for (idx_t k = 0; k <= max_k; ++k) {
        sum += term;
        if (k == max_k) break;        /* no further term */
        term = next_term(term, n, k);
    }
    return sum;
}
