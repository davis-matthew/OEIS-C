/*
 * A343258  Numbers whose binary representation has a prime number of zeros
 *          and a prime number of ones.
 * https://oeis.org/A343258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 8365, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers whose binary representation has a prime number of
   zeros and a prime number of ones. */

#include <stdbool.h>
#include <stddef.h>

/* Very small prime test – enough for the counts of ones/zeros (≤ 128). */
static inline bool isPrimeSmall(int v)
{
    if (v < 2) return false;
    if (v % 2 == 0) return v == 2;
    for (int d = 3; d * d <= v; d += 2)
        if (v % d == 0) return false;
    return true;
}

/* Return the nth term of A343258 (n ≥ 1). */
seq_t a(idx_t n)
{
    /* We produce terms lazily by scanning positive integers. */
    seq_t candidate = 0;   /* will be incremented before first test */

    /* Count of how many valid numbers we have seen.  idx_t is wide enough. */
    idx_t seen = 0;

    while (true)
    {
        /* Next integer to test. */
        candidate += 1;

        /* Count ones and total bits. */
        seq_t x = candidate;
        int ones  = 0;
        int bits  = 0;

        while (x != (seq_t)0)
        {
            if (x & (seq_t)1) ++ones;
            bits++;
            x >>= 1;            /* shift by 1 (int) – OK for arbitrary-precision types */
        }

        int zeros = bits - ones;

        if (isPrimeSmall(ones) && isPrimeSmall(zeros))
        {
            ++seen;
            if (seen == n) return candidate;
        }
    }
}
