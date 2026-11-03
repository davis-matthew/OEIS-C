/*
 * A391757  a(n) = Sum_{k=0..floor(n/4)} 2^k * 3^(n-3*k) *
 *          binomial(2*(n-3*k),2*k).
 * https://oeis.org/A391757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} 2^k * 3^(n-3*k) *
 *            binomial(2*(n-3*k), 2*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 941 (2026-11-01)
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* When n is small the sum contains few terms, otherwise the loop
       runs up to floor(n/4) iterations – for n≤10,000 that is ≤2,500
       iterations, easily fast enough in C or C++.  All operations
       are performed using seq_t arithmetic, which handles arbitrary
       precision when built in the big‑int mode. */
    idx_t maxk = n / 4;
    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t e = n - 3 * k;                     /* exponent of 3 */
        seq_t term = pow2(k);                    /* 2^k */
        term = term * pow3(e);                  /* * 3^(n-3k) */
        term = term * binomial(2 * e, 2 * k);   /* * binomial(...) */
        sum = sum + term;
    }
    return sum;
}
