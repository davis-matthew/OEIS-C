/*
 * A345273  a(n) = Sum_{d|n} (n-d)^tau(d).
 * https://oeis.org/A345273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n-d)^tau(d) (OEIS A345273)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   build_spf, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"   /* provides build_spf(idx_t limit, idx_t **spf_out) */
#include "../utils/seq_pow.h"    /* provides seq_pow(seq_t base, idx_t exp) */

static uint32_t *tau_cache = NULL;      /* tau_cache[i] = tau(i), 1‑based */
static idx_t    tau_cap   = 0;           /* maximum index computed in tau_cache */
static idx_t   *spf_cache = NULL;        /* smallest prime factor array */
static idx_t    spf_cap   = 0;           /* size of spf_cache (maximum index) */

/* ------------------------------------------------------------------
   Rebuild tau_cache and spf_cache up to at least n.
   ------------------------------------------------------------------ */
static void
ensure_tau_upto(idx_t n)
{
    if (n <= tau_cap)          /* already computed */
        return;

    /* free any previously allocated memory */
    free(tau_cache);
    free(spf_cache);

    /* Build smallest prime factor table up to n */
    build_spf(n, &spf_cache);          /* spf_cache[1..n] filled */
    spf_cap = n + 1;

    /* Allocate tau cache */
    tau_cache = (uint32_t *)malloc(sizeof(uint32_t) * (n + 1));
    if (!tau_cache)                  /* out of memory – abort */
        return;

    tau_cache[1] = 1;                 /* tau(1) = 1 */

    /* Compute tau for each integer by factorisation via spf */
    for (idx_t i = 2; i <= n; ++i) {
        idx_t x = i;
        uint32_t tau = 1;
        while (x > 1) {
            idx_t p = spf_cache[x];
            uint32_t cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
            }
            tau *= (cnt + 1);
        }
        tau_cache[i] = tau;
    }

    tau_cap = n;
}

/* ------------------------------------------------------------------
   Main function implementing OEIS sequence A345273.
   ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 0)            /* out of range, return 0 safely */
        return 0;

    /* Compute tau data up to n if necessary */
    ensure_tau_upto(n);

    seq_t sum = 0;

    /* Enumerate divisors of n by looping up to sqrt(n) */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            idx_t d1 = i;
            seq_t term1 = seq_pow((seq_t)(n - d1), (idx_t)tau_cache[d1]);
            sum += term1;

            /* paired divisor d2 = n / i, if distinct */
            idx_t d2 = n / i;
            if (d2 != d1) {
                seq_t term2 = seq_pow((seq_t)(n - d2), (idx_t)tau_cache[d2]);
                sum += term2;
            }
        }
    }

    return sum;
}
