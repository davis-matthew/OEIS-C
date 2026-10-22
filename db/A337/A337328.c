/*
 * A337328  Number of pairs of squarefree divisors of n, (d1,d2), such that d1
 *          <= d2.
 * https://oeis.org/A337328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^k * (2^k + 1) / 2, where k = number of distinct prime
 *            factors of n
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Small list of primes up to 100 – sufficient for n <= 10000. */
static const uint64_t SMALL_PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97
};
static const size_t SMALL_PRIMES_CNT =
    sizeof(SMALL_PRIMES) / sizeof(SMALL_PRIMES[0]);

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 – return 0 safely */

    uint64_t tmp = (uint64_t)n;   /* work on a 64‑bit copy for factoring */
    seq_t m = (seq_t)1;           /* m = 2^k where k counts distinct primes */

    for (size_t i = 0; i < SMALL_PRIMES_CNT; ++i) {
        uint64_t p = SMALL_PRIMES[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            m *= (seq_t)2;          /* include this prime once for squarefree divisors */
            while (tmp % p == 0)    /* remove all copies of p */
                tmp /= p;
        }
    }
    if (tmp > 1)                    /* remaining prime factor > sqrt(original n) */
        m *= (seq_t)2;

    /* Number of unordered pairs of squarefree divisors = N*(N+1)/2 with N=m. */
    seq_t result = m * (m + (seq_t)1) / (seq_t)2;
    return result;
}
