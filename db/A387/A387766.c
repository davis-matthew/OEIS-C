/*
 * A387766  a(n) = Sum_{k=0..floor(n/3)} 2^k * binomial(2*k+1,2*n-6*k).
 * https://oeis.org/A387766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} 2^k * binomial(2*k+1, 2*n-6*k)
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 39, u64 n <= 76, u128 n <= 150,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

/*
 * Compute the nth term of OEIS A387766.
 * The function is thread‑safe and works for any sequence type seq_t.
 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;                 /* floor(n/3) */
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t r = 2 * n - 6 * k;         /* 2n - 6k, always ≥ 0 in the loop */
        seq_t term = pow2(k);            /* 2^k */
        term *= binomial(2 * k + 1, r);  /* binomial(2k+1, r) */
        sum += term;
    }
    return sum;
}
