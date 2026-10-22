/*
 * A359306  Number of divisors of 6*n-2 of form 6*k+1.
 * https://oeis.org/A359306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of (6*n-2) that are ≡ 1 (mod 6)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function factors 6*n-2, removes the factor 2, then
 * counts divisors that are coprime to 6 and congruent to 1 modulo 6
 * by using the parity of exponents of primes ≡ 5 (mod 6).  */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Compute m = 6*n - 2.  Use 64‑bit arithmetic; n will not be
     * so large that 6*n overflows 64 bits in the test data. */
    uint64_t n64 = (uint64_t)n;
    uint64_t m = 6ULL * n64 - 2ULL;

    /* Remove all factors of 2 – only odd divisors can be ≡ 1 (mod 6). */
    while ((m & 1ULL) == 0ULL)
        m >>= 1;

    /* Factor the odd part by trial division.  The size of m is small
     * in the test data (≤ 6*10000≈60000), so this is fast. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (m > 1) {           /* remaining prime factor */
        primes[cnt] = m;
        exps[cnt] = 1;
        ++cnt;
    }

    /* Compute the product of (exp+1) and the parity–adjusted product. */
    seq_t total = 1;      /* product of all (exp+1) */
    seq_t diff  = 1;      /* product of (a_i - b_i) */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        seq_t ways = (seq_t)e + 1;   /* total exponent choices for this prime */
        total *= ways;

        if ((p % 6ULL) == 5ULL) {
            /* For primes ≡ 5 (mod 6) we need to consider parity of the exponent.
             * a_even = number of even exponents, b_odd = number of odd exponents. */
            seq_t a_even = (seq_t)(e / 2 + 1);   /* exponents 0,2,4,... */
            seq_t b_odd  = (seq_t)((e + 1) / 2); /* exponents 1,3,5,... */
            diff *= (a_even - b_odd);           /* a - b */
        } else { /* p % 6 == 1 */
            diff *= ways;   /* a - b = (exp+1) */
        }
    }

    /* The number of divisors ≡ 1 (mod 6) is (total + diff) / 2. */
    return (total + diff) / (seq_t)2;
}
