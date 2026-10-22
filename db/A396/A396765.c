/*
 * A396765  Number of distinct types of connected cyclic Alexander quandles of
 *          order 2n-1.
 * https://oeis.org/A396765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ ord_{2n-1}(t) : 0 <= t < 2n-1,
 *            gcd(t,2n-1)=gcd(1-t,2n-1)=1 }
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   factor_u64, gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Required utilities */
#include "../utils/factor_u64.h"            /* factor_u64 */
#include "../utils/gcd.h"                   /* gcd */
#include "../utils/mod_pow.h"               /* mod_pow */

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    if (x < y) return -1;
    if (x > y) return  1;
    return 0;
}

static void gen_divisors(size_t idx, uint64_t cur,
                         const uint64_t *primes, const uint64_t *exps,
                         size_t cnt, uint64_t *out, size_t *outcnt)
{
    if (idx == cnt) {
        out[(*outcnt)++] = cur;
        return;
    }
    uint64_t p = primes[idx];
    uint64_t val = cur;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        gen_divisors(idx + 1, val, primes, exps, cnt, out, outcnt);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    /* Handle the tiny cases directly */
    if (n <= 1) return 1;

    idx_t m = 2 * n - 1;          /* odd modulus */
    uint64_t mm = (uint64_t)m;
    uint64_t primes_m[64], exps_m[64];
    size_t cnt_m = 0;
    factor_u64(mm, primes_m, exps_m, &cnt_m);

    /* Compute φ(m) */
    seq_t phi = (seq_t)mm;
    for (size_t i = 0; i < cnt_m; ++i) {
        uint64_t p = primes_m[i];
        phi = phi / (seq_t)p * (seq_t)(p - 1);
    }

    /* Factor φ(m) */
    uint64_t phi64 = (uint64_t)phi;
    uint64_t primes_phi[64], exps_phi[64];
    size_t cnt_phi = 0;
    factor_u64(phi64, primes_phi, exps_phi, &cnt_phi);

    /* Generate all divisors of φ(m) */
    uint64_t divs[8192];          /* 2^13 > 20000 divisors upper bound */
    size_t div_cnt = 0;
    gen_divisors(0, 1ULL, primes_phi, exps_phi, cnt_phi, divs, &div_cnt);
    qsort(divs, div_cnt, sizeof(uint64_t), cmp_uint64);

    /* Mark distinct orders */
    bool *seen = (bool *)calloc((size_t)phi64 + 1, sizeof(bool));
    if (!seen) return 0;  /* fallback for OOM */

    seq_t distinct = 0;
    for (idx_t t = 2; t < m; ++t) {
        if (gcd((seq_t)t, (seq_t)m) != (seq_t)1) continue;
        if (gcd((seq_t)(t - 1), (seq_t)m) != (seq_t)1) continue;

        seq_t ord = 0;
        for (size_t k = 0; k < div_cnt; ++k) {
            uint64_t d = divs[k];
            if (mod_pow((seq_t)t, d, (seq_t)m) == (seq_t)1) {
                ord = (seq_t)d;
                break;
            }
        }
        if (ord == (seq_t)0) continue;  /* safety, should not occur */
        if (!seen[(size_t)ord]) {
            seen[(size_t)ord] = true;
            distinct = distinct + 1;
        }
    }

    free(seen);
    return distinct;
}
