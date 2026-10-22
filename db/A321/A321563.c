/*
 * A321563  a(n) = Sum_{d divides n} (-1)^(d + n/d) * d^7.
 * https://oeis.org/A321563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(d + n/d) * d^7
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 21, i64 n <= 512,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void enumerate_divisors(const uint64_t *primes, const int *exps,
                               int pf_cnt, uint64_t cur, uint64_t orig,
                               __int128 *sum)
{
    if (pf_cnt == 0) {
        uint64_t d = cur;
        uint64_t q = orig / d;
        int parity = (int)((d + q) & 1U);          /* 0 or 1 */
        __int128 term = 1;
        for (int i = 0; i < 7; ++i)
            term *= (__int128)d;
        if (parity)
            term = -term;
        *sum += term;
        return;
    }

    uint64_t p = primes[0];
    int emax = exps[0];
    uint64_t next = cur;
    for (int e = 0; e <= emax; ++e) {
        enumerate_divisors(primes + 1, exps + 1, pf_cnt - 1,
                           next, orig, sum);
        next *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                         /* offset 1; n>=1 in usage */
        return 0;

    uint64_t nn = (uint64_t)n;

    /* Factor n by trial division (sufficient for n ≤ 10 000) */
    uint64_t primes[16];
    int exps[16];
    int pf_cnt = 0;

    uint64_t temp = nn;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pf_cnt] = p;
            exps[pf_cnt]   = e;
            ++pf_cnt;
        }
    }
    if (temp > 1) {                     /* remaining prime factor */
        primes[pf_cnt] = temp;
        exps[pf_cnt]   = 1;
        ++pf_cnt;
    }

    __int128 sum = 0;
    enumerate_divisors(primes, exps, pf_cnt, 1ULL,
                       nn, &sum);

    return (seq_t)sum;                  /* cast truncates if necessary */
}
