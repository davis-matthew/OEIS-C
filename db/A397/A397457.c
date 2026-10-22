/*
 * A397457  a(n) = Product_{k=1..n} (prime(n) + 1/k).
 * https://oeis.org/A397457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} (prime(n) + 1/k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/gcd.h"

/* Computes the nth term of OEIS sequence A397457. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    uint32_t prime_val = get_prime(n);
    seq_t a_val = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t D = (seq_t)k;
        seq_t B = (seq_t)prime_val * D + (seq_t)1;

        seq_t g1 = gcd(a_val, D);
        if (g1 > 1)
        {
            a_val /= g1;
            D /= g1;
        }

        seq_t g2 = gcd(B, D);
        if (g2 > 1)
        {
            B /= g2;
            D /= g2;
        }

        /* At this point D should be 1. */
        a_val *= B;
    }

    return a_val;
}
