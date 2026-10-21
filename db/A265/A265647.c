/*
 * A265647  Smallest k such that n divides k*(k+1)*(k+2)/6.
 * https://oeis.org/A265647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 1 such that n | k*(k+1)*(k+2)/6
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiply modulo where the operands are less than mod
   using unsigned __int128 to avoid overflow. */
static inline seq_t mul_mod(seq_t a, seq_t b, seq_t mod)
{
    return (seq_t)((unsigned __int128)a * b % mod);
}

seq_t a(idx_t n)
{
    /* We expect n ≥ 1.  If n == 0, return 0 as a fall‑back. */
    if (n == 0)
        return 0;

    /* We look for k such that k(k+1)(k+2) is divisible by 6*n.
       Testing divisibility by 6*n is equivalent to the original
       condition because the product of three consecutive integers
       is always a multiple of 6. */
    seq_t M = (seq_t)6 * n;               /* M = 6*n, fits in seq_t */
    idx_t k = 1;

    for (;; ++k)
    {
        /* Compute the product modulo M in a way that never overflows seq_t. */
        seq_t a1 = (seq_t)(k % M);
        seq_t a2 = (seq_t)((k + 1) % M);
        seq_t a3 = (seq_t)((k + 2) % M);

        seq_t prod = mul_mod(a1, a2, M);
        prod = mul_mod(prod, a3, M);

        if (prod == 0)          /* 6*n divides k(k+1)(k+2) */
            return (seq_t)k;    /* minimal such k is returned */
    }
}
