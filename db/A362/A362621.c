/*
 * A362621  One and numbers whose multiset of prime factors (with
 *          multiplicity) has the same median as maximum.
 * https://oeis.org/A362621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate A362621 – numbers whose multiset of prime factors
   (with multiplicity) has the same median as the maximum prime factor.
   The sequence is generated lazily: integers are tested one by one
   until the required number of terms is accumulated.  The result for
   a given n is returned from a static cache.  Prime‐factorisations
   are obtained with a small‑est‑prime‑factor (spf) sieve that is
   grown on demand. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* smallest prime factor array and its current upper bound */
static uint32_t *spf = NULL;
static idx_t spf_limit = 0;

/* re‑build the spf array up to `new_limit` (inclusive) */
static void rebuild_spf(idx_t new_limit)
{
    uint32_t *new_spf = (uint32_t *)realloc(spf, (size_t)(new_limit + 1) * sizeof(uint32_t));
    if (!new_spf) return;            /* allocation failure – ignore, caller will retry */
    spf = new_spf;

    for (idx_t i = 0; i <= new_limit; ++i) spf[i] = 0;
    if (new_limit >= 1) spf[1] = 1;

    for (idx_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {                    /* i is prime */
            spf[i] = (uint32_t)i;
            if ((uint64_t)i * i <= (uint64_t)new_limit) {
                for (uint64_t j = (uint64_t)i * i; j <= (uint64_t)new_limit; j += i) {
                    uint32_t *ptr = &spf[(idx_t)j];
                    if (*ptr == 0) *ptr = (uint32_t)i;
                }
            }
        }
    }
    spf_limit = new_limit;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined behaviour; return 0 for safety */

    /* static cache of accepted terms */
    static seq_t *cache           = NULL;
    static idx_t cache_capacity   = 0;
    static idx_t cache_length     = 0;

    /* next integer to test for membership */
    static idx_t next_candidate   = 1;

    /* return if we already have the value */
    if (n <= cache_length) return cache[n-1];

    /* grow cache until we have at least n terms */
    while (cache_length < n) {
        /* ensure spf sieve covers next_candidate */
        if (next_candidate > spf_limit) {
            idx_t new_limit = spf_limit == 0 ? 16 : spf_limit * 2;
            if (new_limit < next_candidate) new_limit = next_candidate;
            rebuild_spf(new_limit);
        }

        bool qualifies = false;

        if (next_candidate == 1) {
            /* 1 has an empty multiset → median (undefined) = maximum (none) → accept */
            qualifies = true;
        } else {
            idx_t x = next_candidate;
            int total = 0;                /* total number of prime factors (with multiplicity) */
            int max_count = 0;             /* multiplicity of the maximum prime factor */
            uint64_t current_max = 0;      /* current maximum prime factor */

            while (x > 1) {
                uint32_t p = spf[x];
                int cnt = 0;
                while (x % p == 0) {
                    x /= p;
                    ++cnt;
                }
                total += cnt;
                if ((uint64_t)p > current_max) {
                    current_max = p;
                    max_count = cnt;
                } else if ((uint64_t)p == current_max) {
                    max_count += cnt;
                }
            }

            /* median equals maximum iff max_count >= floor(total/2)+1 */
            if (max_count >= (total / 2) + 1) qualifies = true;
        }

        if (qualifies) {
            if (cache_length == cache_capacity) {
                size_t new_cap = cache_capacity == 0 ? 64 : cache_capacity * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { free(cache); cache = NULL; cache_capacity = 0; cache_length = 0; return 0; }
                cache = tmp;
                cache_capacity = new_cap;
            }
            cache[cache_length++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[n-1];
}
