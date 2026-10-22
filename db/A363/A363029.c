/*
 * A363029  Expansion of Sum_{k>0} k * x^(4*k-2) / (1 - x^(5*k-3)).
 * https://oeis.org/A363029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5) * sum_{d | 5*n-2, d == 2 (mod 5)} (d + 3) (sum over
 *            positive divisors d of 5*n-2 congruent to 2 modulo 5)
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes term a(n) of OEIS sequence A363029.
   idx_t (unsigned 64-bit) and seq_t are defined in the build environment. */
seq_t a(idx_t n)
{
    /* 5*n-2 comfortably fits in 64‑bit for all tested n (≈10 000). */
    uint64_t N = 5 * n - 2;

    /* Use a 128‑bit integer for the running sum to avoid overflow for larger n. */
    unsigned __int128 sum = 0;

    for (uint64_t i = 1; (unsigned __int128)i * i <= N; ++i)
    {
        if (N % i == 0)
        {
            /* i is a divisor. */
            if ((i % 5) == 2)
                sum += (unsigned __int128)(i + 3);

            uint64_t j = N / i;  /* paired divisor */
            if (j != i && (j % 5) == 2)
                sum += (unsigned __int128)(j + 3);
        }
    }

    /* The sum is guaranteed to be a multiple of 5. */
    seq_t result = (seq_t)(sum / 5);
    return result;
}
