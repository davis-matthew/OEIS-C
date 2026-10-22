/*
 * A318891  Filter sequence combining the prime signature of n (A046523) with
 *          the largest prime factor of n (A006530).
 * https://oeis.org/A318891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 423, u16 n <= 9073, u32 n <= 9073, u64 n <= 9073,
 *            u128 n <= 9073, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Filter sequence of (prime signature, largest prime factor). The
   value a(n) is the rank (starting at 1) of the pair (A046523(n),
   A006530(n)) in the order in which such pairs first appear.  */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Key structure: largest prime factor, sorted exponents, and rank. */
struct seq_key {
    uint64_t lpf;   /* largest prime factor */
    size_t   len;   /* number of distinct prime factors */
    seq_t    rank;  /* rank assigned to this key */
    uint8_t *exps;  /* sorted exponents, descending (len entries) */
};

/* Global tables. */
static struct seq_key *keys      = NULL; /* array of seen keys           */
static size_t          keys_cap  = 0;
static size_t          keys_cnt  = 0;

static seq_t *cache = NULL;          /* a(n) values for n ≥ 1        */
static size_t cache_cap = 0;
static idx_t  cache_max  = 0;         /* highest n that has been cached */

/* Grow the key table to accommodate 'need' elements. */
static void grow_keys(size_t need)
{
    if (keys_cap >= need) return;
    size_t new_cap = keys_cap ? keys_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    struct seq_key *tmp = (struct seq_key *)realloc(keys, new_cap * sizeof(struct seq_key));
    if (!tmp) abort();                /* out of memory */
    keys = tmp;
    keys_cap = new_cap;
}

/* Grow the cache to accommodate indices up to 'need'. */
static void grow_cache(size_t need)
{
    if (cache_cap > need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Compare two keys for equality. */
static bool equal_key(const struct seq_key *a, const struct seq_key *b)
{
    if (a->lpf != b->lpf) return false;
    if (a->len != b->len) return false;
    for (size_t i = 0; i < a->len; ++i)
        if (a->exps[i] != b->exps[i]) return false;
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                 /* undefined for n=0 */

    /* Expand cache if necessary. */
    if (n > cache_max) {
        grow_cache((size_t)n);
        for (idx_t m = cache_max + 1; m <= n; ++m) {
            struct seq_key key;
            if (m == 1) {                      /* special case n = 1 */
                key.lpf = 1;
                key.len = 0;
                key.exps = NULL;
            } else {
                /* Factor m. */
                uint64_t primes[64];
                uint64_t exps[64];
                size_t cnt = 0;
                factor_u64((uint64_t)m, primes, exps, &cnt);
                key.lpf = primes[cnt - 1];     /* largest prime factor */

                /* Sort exponents descending. */
                uint8_t local_exp[64];
                for (size_t i = 0; i < cnt; ++i)
                    local_exp[i] = (uint8_t)exps[i];
                for (size_t i = 1; i < cnt; ++i) {
                    uint8_t v = local_exp[i];
                    size_t j = i;
                    while (j > 0 && local_exp[j-1] < v) {
                        local_exp[j] = local_exp[j-1];
                        --j;
                    }
                    local_exp[j] = v;
                }
                key.len = cnt;
                key.exps = NULL;
                if (cnt) {
                    key.exps = (uint8_t *)malloc(cnt * sizeof(uint8_t));
                    if (!key.exps) abort();
                    for (size_t i = 0; i < cnt; ++i)
                        key.exps[i] = local_exp[i];
                }
            }

            /* Search for existing key. */
            size_t found = keys_cnt;
            for (size_t i = 0; i < keys_cnt; ++i) {
                if (equal_key(&keys[i], &key)) {
                    found = i;
                    break;
                }
            }

            seq_t rank;
            if (found == keys_cnt) {                /* new key */
                grow_keys(keys_cnt + 1);
                keys[keys_cnt].lpf = key.lpf;
                keys[keys_cnt].len = key.len;
                keys[keys_cnt].rank = (seq_t)(keys_cnt + 1);
                keys[keys_cnt].exps = key.exps;      /* take ownership */
                key.exps = NULL;                     /* avoid double free */
                rank = keys[keys_cnt].rank;
                ++keys_cnt;
            } else {                                 /* existing key */
                rank = keys[found].rank;
                if (key.exps) free(key.exps);        /* free temp allocation */
            }

            cache[(size_t)m] = rank;
        }
        cache_max = n;
    }

    return cache[(size_t)n];
}
