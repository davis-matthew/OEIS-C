/*
 * A389459  Numbers whose sum of nonsquarefree divisors (A162296) is congruent
 *          to 2 modulo 4.
 * https://oeis.org/A389459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 2391, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A389459: numbers whose sum of nonsquarefree divisors is ≡2 (mod 4).
   Computed by an incremental sieve and local prime‑factor checks.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Adjust this limit if you need to generate terms beyond 2 000 000. */
#define MAX_LIMIT 2000000

static idx_t *spf = NULL;          /* smallest prime factor for each number */

/* Build the SPF array up to 'limit'.  */
static void build_spf(idx_t limit)
{
    spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i > limit / i) continue;
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    spf[1] = 1;
}

/* Return true iff 'm' satisfies the condition: the sum of its nonsquarefree
   divisors is congruent to 2 modulo 4.  All operations are performed modulo 4. */
static inline bool passes(idx_t m)
{
    seq_t sigma_mod4 = 1;  /* product of σ(pᵉ) modulo 4, over all prime powers */
    seq_t sqf_mod4   = 1;  /* product of (1+p) modulo 4, over all distinct primes */

    idx_t tmp = m;
    while (tmp > 1) {
        idx_t p = spf[tmp];
        size_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }

        /* Compute σ(pᵉ) modulo 4 by summing the geometric series. */
        seq_t sigma_pe = 1;
        seq_t pow = 1;
        seq_t pm = p & 3;   /* p modulo 4 */
        for (size_t i = 0; i < e; ++i) {
            pow = (pow * pm) & 3;
            sigma_pe = (sigma_pe + pow) & 3;
        }
        sigma_mod4 = (sigma_mod4 * sigma_pe) & 3;

        /* Multiply (1+p) modulo 4 to the square‑free sum. */
        seq_t sf_part = ((seq_t)1 + p) & 3;
        sqf_mod4 = (sqf_mod4 * sf_part) & 3;
    }

    seq_t diff = (sigma_mod4 + 4 - sqf_mod4) & 3;
    return diff == 2;
}

/* The main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is defined for n ≥ 1 */

    /* Build the SPF table once. */
    static bool built = false;
    if (!built) {
        build_spf(MAX_LIMIT);
        built = true;
    }

    /* Static cache for generated sequence values. */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static idx_t next_to_test = 1;   /* next integer to evaluate */

    /* Grow the cache array if necessary. */
    if (cap < n) {
        size_t new_cap = cap ? cap : 16;
        while (new_cap < n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        if (passes(next_to_test))
            cache[len++] = (seq_t)next_to_test;
        ++next_to_test;
    }

    return cache[n - 1];
}
