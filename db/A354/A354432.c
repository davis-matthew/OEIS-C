/*
 * A354432  a(n) is the numerator of the sum of the reciprocals of the
 *          nonprime divisors of n.
 * https://oeis.org/A354432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of sum_{d|n, d nonprime} 1/d
 * Range:     u8 n <= 17, u16 n <= 1035, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 91 terms through n = 9494 (2026-11-03)
 * Helpers:   factor_u64, gcd, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
