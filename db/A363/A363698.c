/*
 * A363698  a(n) = n! * Sum_{d|n} (-1)^(d+1) * (n/d)^n / d!.
 * https://oeis.org/A363698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{d|n} (-1)^(d+1) * (n/d)^n / d!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, fact_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/fact_local.h"

/* Recursive divisor enumeration */
static void enumerate_divs(size_t idx, uint64_t cur, uint64_t *pr, int *ex, size_t cnt,
                           idx_t n, seq_t factorial_n, seq_t *res) {
    if (idx == cnt) {
        seq_t base = (seq_t)(n / cur);          /* n/d   */
        seq_t power = big_pow(base, (seq_t)n);  /* (n/d)^n */
        seq_t fact_d = fact_local(cur);         /* d!    */
        seq_t q = factorial_n / fact_d;         /* n!/d! */
        seq_t term = q * power;                 /* term   */
        if (cur & 1ULL) {                       /* odd divisor: sign + */
            *res += term;
        } else {                                /* even divisor: sign - */
            *res -= term;
        }
        return;
    }
    uint64_t p = pr[idx];
    for (int e = 0; e <= ex[idx]; ++e) {
        enumerate_divs(idx + 1, cur, pr, ex, cnt, n, factorial_n, res);
        cur *= p;                     /* increase exponent of p */
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* offset handling */

    /* Compute n! */
    seq_t factorial_n = 1;
    for (idx_t i = 2; i <= n; ++i) factorial_n *= (seq_t)i;

    /* Factorise n into primes and exponents */
    uint64_t primes[64];
    int exps[64];
    size_t cnt = 0;
    uint64_t temp = n;
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (temp > 1) {  /* final prime factor */
        primes[cnt] = temp;
        exps[cnt] = 1;
        ++cnt;
    }

    /* Sum over divisors */
    seq_t result = 0;
    enumerate_divs(0, 1ULL, primes, exps, cnt, n, factorial_n, &result);
    return result;
}
