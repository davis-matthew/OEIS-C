/*
 * A324004  Denominator of 24*Stirling_2(n,4)/n!.
 * https://oeis.org/A324004
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of (24·Stirling_2(n,4))/n!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 23, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/big_pow.h"     /* big_pow(seq_t base, seq_t exp)   */
#include "../utils/seq_mul.h"     /* seq_mul(seq_t a, seq_t b)      */
#include "../utils/seq_pow.h"     /* seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* undefined for n < 4; return 0 */
    if (n < 4) return 0;

    /* ---------- compute numerator 4^n - 4·3^n + 6·2^n - 4 ---------- */
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);

    seq_t numerator = pow4;
    numerator = seq_mul(numerator, (seq_t)1);      /* keep type‑consistent */

    numerator = numerator - (pow3 * (seq_t)4);
    numerator = numerator + (pow2 * (seq_t)6);
    numerator = numerator - (seq_t)4;

    /* ---------- sieve primes up to n ---------- */
    size_t limit = (size_t)n;
    bool *is_comp = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_comp) return 0;                     /* allocation failed */
    size_t *primes = (size_t*)malloc(limit * sizeof(size_t));
    size_t primes_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }

    /* ---------- compute denominator from prime exponents ---------- */
    seq_t denom = (seq_t)1;
    for (size_t idx = 0; idx < primes_cnt; ++idx) {
        idx_t p = primes[idx];

        /* exponent of p in n!  (Legendre) */
        idx_t exp_n = 0;
        for (idx_t pp = p; pp <= n; pp *= p) {
            exp_n += n / pp;
            if (pp > n / p) break;            /* avoid overflow of pp */
        }

        /* exponent of p in the numerator */
        seq_t temp = numerator;
        idx_t exp_num = 0;
        while ((temp % p) == 0) {
            temp /= p;
            ++exp_num;
        }

        idx_t exp_gcd = (exp_n < exp_num) ? exp_n : exp_num;
        idx_t exp_den = exp_n - exp_gcd;
        if (exp_den > 0) {
            denom = seq_mul(denom, seq_pow((seq_t)p, exp_den));
        }
    }

    /* ---------- cleanup ---------- */
    free(is_comp);
    free(primes);

    return denom;
}
