/*
 * A344194  a(n) = Sum_{k=1..n} tau(gcd(k,n))^gcd(k,n), where tau(n) is the
 *          number of divisors of n.
 * https://oeis.org/A344194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * tau(d)^d
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-01)
 * Helpers:   build_primes_upto, ipow, num_divisors, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>

/* Utilities provided by the build system */
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow.h"
#include "../utils/num_divisors.h"
#include "../utils/totient_seq.h"

/* ------------------------------------------------------------------ */
static void
_generate_divisors(const uint64_t *primes,
                   const int      *exps,
                   size_t          idx,
                   size_t          num_factors,
                   uint64_t        acc,
                   uint64_t        *dst,
                   size_t          *len)
{
    if (idx == num_factors) {
        dst[(*len)++] = acc;
        return;
    }

    uint64_t p   = primes[idx];
    uint64_t mult = 1;
    for (int e = 0; e <= exps[idx]; ++e) {
        _generate_divisors(primes, exps, idx + 1, num_factors,
                           acc * mult, dst, len);
        mult *= p;
    }
}

/* ------------------------------------------------------------------ */
/* Recursively generate all divisors of n from its prime factorisation */

/* ------------------------------------------------------------------ */
/* Main sequence function                                                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) {                /* offset starts at 1 – never used but harmless */
        return 0;
    }

    /* ------------------------------------------------------------------ */
    /* Prime cache: grow once per maximum n encountered                      */
    /* ------------------------------------------------------------------ */
    static uint64_t *primes  = NULL;
    static size_t    prime_cnt = 0;
    static idx_t     primes_upto = 0;

    if (n > primes_upto) {
        if (primes) {
            free(primes);
        }
        build_primes_upto(n, &primes, &prime_cnt);
        primes_upto = n;
    }

    /* ------------------------------------------------------------------ */
    /* Factorise n using the cached primes                                  */
    /* ------------------------------------------------------------------ */
    uint64_t temp   = n;
    uint64_t fac_primes[64];   /* 64 is far enough for n ≤ 10⁴                      */
    int      fac_exps[64];
    size_t   fac_cnt = 0;

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= temp; ++i) {
        if (temp % primes[i] == 0) {
            fac_primes[fac_cnt] = primes[i];
            int e = 0;
            while (temp % primes[i] == 0) {
                temp /= primes[i];
                ++e;
            }
            fac_exps[fac_cnt] = e;
            ++fac_cnt;
        }
    }
    if (temp > 1) {                                   /* remaining prime */
        fac_primes[fac_cnt] = temp;
        fac_exps[fac_cnt]   = 1;
        ++fac_cnt;
    }

    /* ------------------------------------------------------------------ */
    /* Generate all divisors of n                                            */
    /* ------------------------------------------------------------------ */
    uint64_t divs[2048];   /* enough for all n ≤ 10⁴                            */
    size_t   div_cnt = 0;
    _generate_divisors(fac_primes, fac_exps, 0, fac_cnt, 1, divs, &div_cnt);

    /* ------------------------------------------------------------------ */
    /* Evaluate the divisor sum                                              */
    /* ------------------------------------------------------------------ */
    seq_t sum = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t d   = divs[i];
        idx_t    nd  = n / d;                           /* n/d fits in idx_t */
        seq_t   phi_val = totient_seq((seq_t)nd);          /* φ(n/d) */
        seq_t   tau_d   = (seq_t)num_divisors(d);  /* τ(d)  */
        seq_t   pow_val = ipow(tau_d, (seq_t)d);     /* τ(d)^d */
        sum += phi_val * pow_val;
    }

    return sum;
}
