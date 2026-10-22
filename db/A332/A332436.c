/*
 * A332436  The number of even numbers <= n of the smallest nonnegative
 *          reduced residue system modulo 2*n + 1, for n >= 0.
 * https://oeis.org/A332436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A332436
   Formula: a(n) = # { even k ≤ n | gcd(k, 2*n+1) = 1 }
   This is equivalent to counting integers m ≤ ⌊n/2⌋ coprime to M = 2*n+1.
   Computation is performed by inclusion–exclusion over the distinct prime
   factors of M.  The function accepts any idx_t (unsigned 64‑bit) and
   returns a seq_t (unsigned integer type chosen by the build). */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0)
        return (seq_t)1;

    uint64_t M = 2ULL * (uint64_t)n + 1ULL;   /* M = 2*n + 1 (odd) */
    uint64_t limit = n / 2ULL;                /* largest odd m: floor(n/2) */

    /* Gather distinct prime factors of M */
    uint64_t primes[32];
    size_t pc = 0;
    uint64_t temp = M;
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            primes[pc++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1)
        primes[pc++] = temp;   /* remaining prime factor */

    /* Inclusion–exclusion over the distinct primes */
    long long result = 0;      /* signed to allow subtraction */
    size_t subsets = 1ULL << pc;   /* 2^pc subsets, pc ≤ 15 for 64‑bit M */

    for (size_t mask = 0; mask < subsets; ++mask)
    {
        uint64_t prod = 1;
        size_t bits = 0;
        for (size_t i = 0; i < pc; ++i)
        {
            if (mask & (1ULL << i))
            {
                prod *= primes[i];
                ++bits;
            }
        }
        long long cnt = limit / prod;
        result += (bits & 1 ? -cnt : cnt);
    }

    return (seq_t)result;
}
