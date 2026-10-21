/*
 * A264778  Numbers k such that 6*k+1, 3*k+2, 6*k+7 are all odd semiprimes.
 * https://oeis.org/A264778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth odd integer k such that 6k+1, 3k+2 and 6k+7 are all
 *            odd semiprimes
 * Range:     u8 n <= 9, u16 n <= 2529, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether a positive odd integer is a semiprime (product of exactly two primes,
   counted with multiplicity). */
static bool is_semiprime_uint64(uint64_t x)
{
    if ((x & 1u) == 0u)          /* must be odd */
        return false;

    uint64_t temp = x;
    int count = 0;

    /* check prime factors starting from 3, skipping multiples of 2 */
    for (uint64_t d = 3; d * d <= temp; d += 2)
    {
        while (temp % d == 0)
        {
            temp /= d;
            ++count;
            if (count > 2)
                return false;
        }
    }
    if (temp > 1)        /* remaining prime factor */
    {
        ++count;
        if (count != 2)
            return false;
    }
    return count == 2;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* The sequence is 1‑based; return 0 for n <= 0 as a safe default. */
    if (n == 0)
        return (seq_t)0;

    uint64_t found = 0;    /* number of terms found so far */
    uint64_t k = 1;        /* start search with the smallest odd integer */

    while (found < n)
    {
        /* Check only odd k – even k cannot give all three numbers odd. */
        if (k & 1u)
        {
            uint64_t n1 = 6 * k + 1;
            uint64_t n2 = 3 * k + 2;
            uint64_t n3 = 6 * k + 7;

            if (is_semiprime_uint64(n1) &&
                is_semiprime_uint64(n2) &&
                is_semiprime_uint64(n3))
            {
                ++found;
                if (found == n)
                    return (seq_t)k;
            }
        }
        ++k;
    }

    /* Control should never reach here. */
    return (seq_t)0;
}
