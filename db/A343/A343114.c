/*
 * A343114  a(n) = Sum_{i=1..n} gcd(n^i,i).
 * https://oeis.org/A343114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1..n} gcd(n^i , i)
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

static idx_t *spf_cache = NULL;
static idx_t spf_limit = 0;

/* Build smallest‑prime‑factor table up to 'limit' and cache it */
static void ensure_spf(idx_t limit)
{
    if (spf_cache && spf_limit >= limit)
        return;                /* table already covers the range */
    if (spf_cache)
        free(spf_cache);       /* discard old table */
    spf_cache = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!spf_cache)
        return;                /* allocation failure – fallback to 0 */
    for (idx_t i = 0; i <= limit; ++i)
        spf_cache[i] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf_cache[i] == 0) {
            spf_cache[i] = i;
            if (i * i <= limit)
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf_cache[j] == 0)
                        spf_cache[j] = i;
        }
    }
    spf_limit = limit;
}

/* Small integer power (base^exp) with seq_t arithmetic */
static seq_t pow_seq(seq_t base, unsigned exp)
{
    seq_t res = 1;
    while (exp--)
        res *= base;
    return res;
}

/* Main routine */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* harmless for unused offset */

    ensure_spf(n);

    /* Factor n once: record its distinct prime factors and exponents */
    int primes_n[16];
    int exp_n[16];
    int pn_cnt = 0;
    idx_t tn = n;
    while (tn > 1) {
        idx_t p = spf_cache[tn];
        int e = 0;
        while (tn % p == 0) {
            tn /= p;
            e++;
        }
        primes_n[pn_cnt] = (int)p;
        exp_n[pn_cnt]   = e;
        pn_cnt++;
    }

    seq_t sum = 0;

    /* Process each i = 1..n */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t g = 1;
        idx_t ti = i;
        while (ti > 1) {
            idx_t p = spf_cache[ti];
            int e = 0;
            while (ti % p == 0) {
                ti /= p;
                e++;
            }
            /* find exponent of p in n (0 if p∤n) */
            int vp_n = 0;
            for (int k = 0; k < pn_cnt; ++k) {
                if (primes_n[k] == (int)p) {
                    vp_n = exp_n[k];
                    break;
                }
            }
            if (vp_n == 0)
                continue;   /* this prime does not contribute to the gcd */

            idx_t prod = i * vp_n;          /* i·v_p(n) */
            int exp = (prod < (idx_t)e) ? (int)prod : e; /* min(i·v_p(n), v_p(i)) */
            g *= pow_seq((seq_t)p, exp);
        }
        sum += g;
    }
    return sum;
}
