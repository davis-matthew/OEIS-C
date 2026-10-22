/*
 * A363614  Expansion of Sum_{k>0} x^(2*k)/(1+x^k)^6.
 * https://oeis.org/A363614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} (-1)^d * binomial(d+3,5)
 * Range:     i8 n <= 6, i16 n <= 19, i32 n <= 190, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Compute binomial(d+3,5); result is 0 if d+3 < 5. */
static inline seq_t binom5(idx_t d) {
    if (d + 3 < 5) return 0;
    __int128 t = (__int128)(d + 3);
    t *= (d + 2);
    t *= (d + 1);
    t *= d;
    t *= (d - 1);
    t /= 120;
    return (seq_t)t;
}

/* Recursively enumerate all divisors and accumulate the sum. */
static void enum_divs(size_t idx, idx_t curr,
                      const idx_t *primes, const int *exps,
                      size_t cnt, seq_t *sum) {
    if (idx == cnt) {
        seq_t term = binom5(curr);
        if (curr & 1)
            *sum -= term; /* odd divisor: sign negative */
        else
            *sum += term; /* even divisor: sign positive */
        return;
    }
    idx_t p = primes[idx];
    idx_t val = curr;
    for (int e = 0; e <= exps[idx]; ++e) {
        enum_divs(idx + 1, val, primes, exps, cnt, sum);
        val *= p;
    }
}

/* Main function computing a(n). */
seq_t a(idx_t n) {
    if (n < 1) return 0; /* offset starts at 1 */

    /* Factor n using trial division (n is at most ~10^4). */
    idx_t tmp = n;
    idx_t primes[20];
    int   exps[20];
    size_t cnt = 0;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (tmp > 1) { /* remaining prime factor */
        primes[cnt] = tmp;
        exps[cnt]   = 1;
        ++cnt;
    }

    seq_t sum = 0;
    enum_divs(0, (idx_t)1, primes, exps, cnt, &sum);
    return sum;
}
