/*
 * A303756  Number of values of k, 1 <= k <= n, with A002322(k) = A002322(n),
 *          where A002322 is Carmichael lambda.
 * https://oeis.org/A303756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 9791, u32 n <= 9791, u64 n <= 9791,
 *            u128 n <= 9791, big any n
 * Helpers:   build_spf, ensure_cache, gcd, mul_small, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A303756
   (number of k, 1 ≤ k ≤ n, with A002322(k) = A002322(n)).
   Implements a sieve of Carmichael λ values up to n with caching. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/ensure_cache.h"
#include "../utils/build_spf.h"
#include "../utils/pow_int.h"
#include "../utils/mul_small.h"

static idx_t max_n = 0;              /* maximum n processed so far          */
static seq_t *lam_cache = NULL;      /* cached λ values, indexed by n      */
static size_t lam_cap = 0;           /* capacity of lam_cache              */
static idx_t *spf = NULL;            /* smallest prime factor array        */
static size_t spf_cap = 0;           /* capacity of spf array              */

/* Compute Carmichael λ(m) using the precomputed spf array. */
static seq_t compute_lambda(idx_t m, idx_t *spf_arr)
{
    if (m == 1) return (seq_t)1;
    seq_t lval = 1;
    idx_t x = m;
    while (x > 1) {
        idx_t p = spf_arr[x];
        idx_t e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }
        seq_t part;
        if (p == 2) {
            if (e == 1)               part = (seq_t)1;
            else if (e == 2)          part = (seq_t)2;
            else                     part = pow_int((seq_t)2, e - 2);
        } else {
            seq_t pow_part = pow_int((seq_t)p, e - 1);
            part = mul_small(pow_part, (unsigned int)(p - 1));
        }
        seq_t g = gcd(lval, part);
        lval = (lval / g) * part;   /* lcm(lval, part) */
    }
    return lval;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */

    /* Extend caches if necessary */
    if (n > max_n) {
        /* Update spf array if needed */
        if (n + 1 > spf_cap) {
            free(spf);
            build_spf(n, &spf);
            spf_cap = n + 1;
        }
        /* Ensure lambda cache capacity */
        if (n + 1 > lam_cap) {
            ensure_cache(n, &lam_cache, &lam_cap);
        }
        /* Compute λ for new indices */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            lam_cache[i] = compute_lambda(i, spf);
        }
        max_n = n;
    }

    /* Count how many k ≤ n have the same λ as n */
    seq_t target = lam_cache[n];
    seq_t cnt = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (lam_cache[i] == target) ++cnt;
    }
    return cnt;
}
