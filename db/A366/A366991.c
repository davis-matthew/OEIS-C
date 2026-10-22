/*
 * A366991  The number of divisors of n that are not terms of A322448.
 * https://oeis.org/A366991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime exponents e of (π(e)+2), where π(e) is
 *            the number of primes ≤ e. This equals the number of divisors of
 *            n that are not terms of A322448
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const uint64_t small_primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL,
    11ULL, 13ULL, 17ULL, 19ULL,
    23ULL, 29ULL, 31ULL, 37ULL,
    41ULL, 43ULL, 47ULL, 53ULL,
    59ULL, 61ULL
};
static const size_t small_primes_cnt =
    sizeof small_primes / sizeof small_primes[0];

static size_t pi_small(uint64_t e)
{
    size_t c = 0;
    for (size_t i = 0; i < small_primes_cnt && small_primes[i] <= e; ++i)
        ++c;
    return c;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t m = (uint64_t)n;
    uint64_t exps[64];
    size_t cnt = 0;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            exps[cnt++] = e;
        }
    }
    if (m > 1) {
        exps[cnt++] = 1;          /* Remaining prime factor */
    }

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)(pi_small(exps[i]) + 2);
        result *= term;
    }
    return result;
}
