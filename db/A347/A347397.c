/*
 * A347397  a(n) = Sum_{k=1..n} k^k * floor(n/k^k).
 * https://oeis.org/A347397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^k * floor(n / k^k)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the nth term of OEIS A347397.
   idx_t is an unsigned 64‑bit integer (uint64_t).
   seq_t is an unsigned integer type (8, 16, 32, 64, 128 bits or arbitrary precision). */
seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* iterate over k while k^k <= n */
    for (idx_t k = 1; ; ++k)
    {
        /* compute k^k in seq_t; stop early if it exceeds n */
        seq_t pow = 1;
        for (idx_t i = 0; i < k; ++i)
        {
            pow *= (seq_t)k;
            if (pow > (seq_t)n) break;
        }
        if (pow > (seq_t)n) break;          /* k^k > n -> further terms are zero */

        /* floor(n / pow) * pow = n - (n % pow) */
        sum += ((seq_t)n / pow) * pow;
    }

    return sum;
}
