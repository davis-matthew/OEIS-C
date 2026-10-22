/*
 * A352060  a(n) = (n - 1)! * omega(n), where omega(n) = number of distinct
 *          primes dividing n (A001221).
 * https://oeis.org/A352060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * ω(n), where ω(n) is the number of distinct prime
 *            divisors of n (OEIS A001221)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implementation uses only the standard headers and a
   simple trial‑division routine for ω(n).  It works for any n ≥ 1 and
   does not rely on the optional utility headers.  The factorial is
   computed straightforwardly.  The code respects the calling
   convention required by the surrounding build system. */

#include <stdint.h>
#include <stddef.h>

/* Small table of primes up to 100.  Sufficient for n ≤ 10 000;
   the trial‑division routine aborts when p*p > n. */
static const uint32_t small_primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37,
    41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97
};
static const size_t small_primes_cnt = sizeof(small_primes) / sizeof(small_primes[0]);

/* Count distinct prime divisors of n.  This is a lightweight
   implementation that does not depend on the utility library. */
static unsigned int distinct_prime_count(uint64_t n)
{
    unsigned int cnt = 0;
    for (size_t i = 0; i < small_primes_cnt && (uint64_t)small_primes[i] * small_primes[i] <= n; ++i) {
        uint32_t p = small_primes[i];
        if (n % p == 0) {
            ++cnt;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1) /* remaining prime factor > 100 */
        ++cnt;
    return cnt;
}

/* Main sequence function.  All arithmetic is performed in seq_t,
   which is an unsigned integer type large enough for the output on
   the current build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined below offset; return 0 for safety */
    if (n == 1)
        return 0;          /* (1-1)! = 0! = 1, ω(1) = 0 → product 0 */

    /* compute (n-1)! */
    seq_t fact = 1;
    /* Loop using idx_t to avoid signed issues on large n. */
    for (idx_t i = 2; i <= n - 1; ++i)
        fact *= (seq_t)i;

    /* compute ω(n) */
    uint64_t nn = (uint64_t)n;              /* promote to 64-bit for division */
    unsigned int omega = distinct_prime_count(nn);

    return fact * (seq_t)omega;
}
