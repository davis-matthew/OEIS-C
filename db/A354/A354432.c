/*
 * A354432  a(n) is the numerator of the sum of the reciprocals of the
 *          nonprime divisors of n.
 * https://oeis.org/A354432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of sum_{d|n, d nonprime} 1/d
 * Range:     u8 n <= 17, u16 n <= 1035, u32 n <= 9494, u64 n <= 9494,
 *            u128 n <= 9494, big any n
 * Helpers:   factor_u64, gcd, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n = 1 is a special case: only divisor is 1 → sum = 1 → numerator = 1 */
    if (n == 1) return (seq_t)1;

    /* ----- factorise n into primes ----- */
    uint64_t pf[64];
    uint64_t ex[64];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)n, pf, ex, &pf_cnt);

    /* ----- generate all divisors of n ----- */
    seq_t divs[256];               /* 256 > tau(max(n<=10000)) */
    size_t divcnt = 1;
    divs[0] = (seq_t)1;            /* divisor 1 is always present */

    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t prime = pf[i];
        unsigned int exp = (unsigned int)ex[i];
        size_t oldcnt = divcnt;
        seq_t ppow = (seq_t)1;
        for (unsigned int e = 1; e <= exp; ++e) {
            ppow *= (seq_t)prime;          /* prime^e */
            for (size_t j = 0; j < oldcnt; ++j) {
                divs[divcnt++] = divs[j] * ppow;
            }
        }
    }

    /* ----- sum reciprocals of the non‑prime divisors in lowest terms ----- */
    seq_t num = (seq_t)0;
    seq_t den = (seq_t)1;

    for (size_t i = 0; i < divcnt; ++i) {
        seq_t d = divs[i];
        /* skip prime divisors (1 is kept) */
        if (d != (seq_t)1 && is_prime_big(d))
            continue;

        seq_t new_num = num * d + den;
        seq_t new_den = den * d;
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    return num;
}
