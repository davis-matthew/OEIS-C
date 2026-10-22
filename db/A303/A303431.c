/*
 * A303431  Aperiodic tree numbers. Matula-Goebel numbers of aperiodic rooted
 *          trees.
 * https://oeis.org/A303431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1. For n>1, a(n) is the nth integer that is not a perfect
 *            power and whose prime indices (indices of its prime factors) are
 *            themselves aperiodic tree numbers
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd_int.h"

/* Global data for incremental sieve and aperiodic number table */
static int      *spf          = NULL; /* smallest prime factor */
static int      *prime_index  = NULL; /* index of each prime (1‑based) */
static int      *is_ap        = NULL; /* 1 if number is aperiodic */
static uint32_t *primes       = NULL; /* list of primes */
static size_t    primes_cnt   = 0;

static seq_t    *ap_vals      = NULL; /* table of aperiodic numbers */
static size_t    ap_cnt       = 0;
static size_t    ap_cap       = 0;

static size_t    curr_limit   = 0;
static size_t    spf_cap      = 0;
static size_t    pi_cap       = 0;
static size_t    is_ap_cap    = 0;
static size_t    primes_cap   = 0;

/* Initialize tables for limit 1 */
static void init_once(void) {
    if (curr_limit > 0) return;

    curr_limit = 1;
    /* allocate boolean table for number 1 */
    is_ap_cap = 2;
    is_ap = (int *)realloc(is_ap, is_ap_cap * sizeof(int));
    is_ap[0] = 0; is_ap[1] = 1;

    spf_cap   = 2;
    spf   = (int *)realloc(spf, spf_cap * sizeof(int));
    spf[0] = 0; spf[1] = 0;

    pi_cap   = 2;
    prime_index = (int *)realloc(prime_index, pi_cap * sizeof(int));
    prime_index[0] = 0; prime_index[1] = 0;

    /* aperiodic table: only 1 so far */
    ap_cap = 1;
    ap_vals = (seq_t *)realloc(ap_vals, ap_cap * sizeof(seq_t));
    ap_vals[0] = (seq_t)1;
    ap_cnt = 1;

    primes_cap = 1;
    primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
    primes_cnt = 0;
}

/* Incrementally extend sieve and aperiodic table up to new_limit (inclusive) */
static void extend_to(size_t new_limit) {
    size_t old_limit = curr_limit;

    /* Resize auxiliary arrays */
    if (spf_cap <= new_limit) {
        spf_cap = new_limit + 1;
        spf = (int *)realloc(spf, spf_cap * sizeof(int));
    }
    if (pi_cap <= new_limit) {
        pi_cap = new_limit + 1;
        prime_index = (int *)realloc(prime_index, pi_cap * sizeof(int));
    }
    if (is_ap_cap <= new_limit) {
        is_ap_cap = new_limit + 1;
        is_ap = (int *)realloc(is_ap, is_ap_cap * sizeof(int));
    }
    if (primes_cap <= new_limit) {
        primes_cap = new_limit + 1;
        primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
    }

    /* initialise new entries to zero */
    for (size_t i = old_limit + 1; i <= new_limit; ++i) {
        spf[i] = 0;
        prime_index[i] = 0;
        is_ap[i] = 0;
    }

    /* Linear sieve – continue from old_limit+1 to new_limit */
    for (size_t i = old_limit + 1; i <= new_limit; ++i) {
        if (spf[i] == 0) {               /* i is prime */
            spf[i] = (int)i;
            if (primes_cnt == primes_cap) {        /* grow prime list */
                primes_cap = primes_cap ? primes_cap * 2 : 4;
                primes = (uint32_t *)
                         realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = (uint32_t)i;
            prime_index[i] = (int)primes_cnt;      /* 1‑based index */
        }
        /* update spf for multiples */
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            if (p > spf[i]) break;
            size_t mul = i * (size_t)p;
            if (mul > new_limit) break;
            spf[mul] = (int)p;
        }
    }

    /* Determine aperiodic numbers in the new range */
    for (size_t n = (old_limit >= 1 ? old_limit + 1 : 2); n <= new_limit; ++n) {
        if (n == 1) continue;          /* already handled */

        /* factor n using spf */
        size_t x = n;
        int exp[64], pk[64];
        int cnt = 0;
        while (x > 1) {
            int p = spf[x];
            int e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            exp[cnt] = e;
            pk[cnt]  = prime_index[p];
            ++cnt;
        }

        /* check perfect power: gcd of exponents > 1 means perfect power */
        int g = exp[0];
        for (int i = 1; i < cnt; ++i) g = gcd_int(g, exp[i]);
        if (g > 1) continue;            /* not aperiodic */

        /* verify all prime indices are aperiodic */
        int ok = 1;
        for (int i = 0; i < cnt; ++i) {
            if (!is_ap[ pk[i] ]) { ok = 0; break; }
        }
        if (!ok) continue;

        /* n is aperiodic */
        is_ap[n] = 1;
        if (ap_cnt == ap_cap) {
            ap_cap = ap_cap ? ap_cap * 2 : 4;
            ap_vals = (seq_t *)realloc(ap_vals, ap_cap * sizeof(seq_t));
        }
        ap_vals[ap_cnt++] = (seq_t)n;
    }

    curr_limit = new_limit;
}

/* Ensure that the table has at least n elements */
static void ensure_size(idx_t n) {
    init_once();
    if (ap_cnt >= n) return;
    size_t limit = curr_limit ? curr_limit * 2 + 1000 : 2000;
    while (ap_cnt < n) {
        if (limit <= curr_limit) limit = curr_limit * 2 + 1000;
        extend_to(limit);
    }
}

/* Public function: returns the nth aperiodic tree number */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined behaviour for n=0 */
    ensure_size(n);
    return ap_vals[n - 1];
}
