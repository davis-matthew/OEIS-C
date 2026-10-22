/*
 * A326052  a(n) = n - A050452(n), where A050452 is the sum of divisors of the
 *          form 4k+3.
 * https://oeis.org/A326052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - A050452(n)
 * Range:     i8 n <= 127, i16 n <= 6624, i32 n <= 6624, i64 n <= 6624,
 *            i128 n <= 6624, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A050452(n) is the sum of divisors of n congruent to 3 mod 4 */

#include <stdint.h>
#include <stdlib.h>

/* maximum number of divisors we expect to generate; safe for all 64‑bit n */
#define MAX_DIVISORS 100000

seq_t a(idx_t n)
{
    /* factor n using trial division */
    idx_t t = n;
    uint64_t primes[64];
    uint32_t exps[64];
    size_t pf_cnt = 0;

    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            uint32_t e = 0;
            do {
                t /= p;
                ++e;
            } while (t % p == 0);
            primes[pf_cnt] = p;
            exps[pf_cnt++] = e;
        }
    }
    if (t > 1) {          /* remaining prime factor */
        primes[pf_cnt] = t;
        exps[pf_cnt]   = 1;
        ++pf_cnt;
    }

    /* generate all divisors of n */
    static uint64_t divs[MAX_DIVISORS];
    size_t div_cnt = 1;
    divs[0] = 1;

    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = primes[i];
        uint32_t e = exps[i];
        size_t curr = div_cnt;
        uint64_t mul = 1;
        for (uint32_t k = 1; k <= e; ++k) {
            mul *= p;
            for (size_t j = 0; j < curr; ++j)
                divs[div_cnt++] = divs[j] * mul;
        }
    }

    /* sum the divisors that are ≡ 3 (mod 4) */
    uint64_t sum = 0;
    for (size_t i = 0; i < div_cnt; ++i)
        if ((divs[i] & 3ULL) == 3ULL)
            sum += divs[i];

    return (seq_t)n - (seq_t)sum;
}
