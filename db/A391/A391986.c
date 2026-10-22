/*
 * A391986  a(n) = Sum_{k=0..floor(n/4)} 2^k * 3^(n-3*k) * binomial(k+2,2) *
 *          binomial(n-2*k,k) * binomial(n-3*k,k).
 * https://oeis.org/A391986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Sum_{k=0..⌊n/4⌋} 2^k * 3^(n-3k) * C(k+2,2) * C(n-2k,k) * C(n-3k,k)
 */
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t maxk = n / 4;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term = pow2(k);                     /* 2^k */
        term *= pow3(n - 3 * k);                  /* * 3^(n-3k) */
        term *= binomial(k + 2, 2);               /* * C(k+2,2) */
        term *= binomial(n - 2 * k, k);           /* * C(n-2k,k) */
        term *= binomial(n - 3 * k, k);           /* * C(n-3k,k) */
        result += term;
    }
    return result;
}
