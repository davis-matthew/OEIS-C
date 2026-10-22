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
 * Helpers:   _generate_divisors, big_pow, build_primes_upto, calc_phi,
 *            divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>

/* Utilities provided by the build system */
#include "../utils/big_pow.h"               /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/build_primes_upto.h"    /* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */
#include "../utils/calc_phi.h"             /* seq_t calc_phi(seq_t m); */
#include "../utils/divisor_count_u64.h"    /* uint64_t divisor_count_u64(uint64_t n); */
#include "../utils/_generate_divisors.h"

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
        seq_t   phi_val = calc_phi((seq_t)nd);          /* φ(n/d) */
        seq_t   tau_d   = (seq_t)divisor_count_u64(d);  /* τ(d)  */
        seq_t   pow_val = big_pow(tau_d, (seq_t)d);     /* τ(d)^d */
        sum += phi_val * pow_val;
    }

    return sum;
}
