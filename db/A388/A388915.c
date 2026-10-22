/*
 * A388915  a(n) = Sum_{k=0..n} 3^k * binomial(n,k) * binomial(2*n+2*k,k).
 * https://oeis.org/A388915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * binomial(n,k) * binomial(2*n+2*k,k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */
#include "../utils/pow3.h"       /* provides seq_t pow3(idx_t exp) */

/* Compute the OEIS sequence A388915 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t power3 = pow3(k);                /* 3^k */
        seq_t c1     = binomial(n, k);         /* C(n,k)   */
        idx_t m      = 2 * n + 2 * k;          /* 2n+2k    */
        seq_t c2     = binomial(m, k);         /* C(2n+2k,k) */
        seq_t prod   = power3 * c1 * c2;       /* product   */
        sum += prod;                           /* accumulate */
    }
    return sum;
}
