/*
 * A371871  a(n) = Sum_{k=0..floor(n/3)} binomial(2*n-3*k-2,n-3*k).
 * https://oeis.org/A371871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*n-3*k-2, n-3*k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the provided binomial(idx_t n, idx_t k)
   helper from "../utils/binomial.h". It handles out‑of‑range
   arguments by returning 0. */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;     /* a(0) = 1 */

    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t nn = 2 * n - 3 * k - 2; /* top of binomial */
        idx_t kk = n - 3 * k;         /* bottom of binomial */
        seq_t val = 0;
        if (kk <= nn)                 /* avoid invalid arguments */
            val = binomial(nn, kk);
        sum += val;
    }
    return sum;
}
