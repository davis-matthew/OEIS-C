/*
 * A339564  Number of ways to choose a distinct factor in a factorization of n
 *          (pointed factorizations).
 * https://oeis.org/A339564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d|n, d>1} A001055(n/d) (A001055 is the number of
 *            unordered factorizations of its argument.)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

struct CacheEntry {
    uint64_t key;   /* (n << 32) | min_factor  */
    seq_t value;    /* cached result f(n, min_factor)   */
    unsigned used;  /* 0 = empty, 1 = occupied   */
};

#define CACHE_CAP 262144   /* power of two for simple indexing */

static struct CacheEntry *cache = NULL;

/* one‑time initialization of the cache */
static void init_cache(void) {
    if (!cache) {
        cache = (struct CacheEntry *)malloc(CACHE_CAP * sizeof(struct CacheEntry));
        for (size_t i = 0; i < CACHE_CAP; ++i)
            cache[i].used = 0;
    }
}

/* recursively count unordered factorizations of n with smallest factor >= min_factor
   (this is A001055 when called with min_factor = 2). */
static seq_t factor_count(idx_t n, idx_t min_factor) {
    if (n < min_factor) return 0;
    if (n == 1) return 1;   /* the empty factorization of 1 */

    init_cache();           /* ensure cache is ready */

    uint64_t key = ((uint64_t)n << 32) | (uint64_t)min_factor;
    size_t pos   = (size_t)(key & (CACHE_CAP - 1));

    /* linear probing lookup */
    while (cache[pos].used) {
        if (cache[pos].key == key) return cache[pos].value;
        pos = (pos + 1) & (CACHE_CAP - 1);
    }

    /* compute the value and store it */
    seq_t result = 1;       /* the factorization consisting only of n itself */

    for (idx_t i = min_factor; (uint64_t)i * i <= n; ++i) {
        if (n % i == 0) {
            result += factor_count(n / i, i);
        }
    }

    cache[pos].used  = 1;
    cache[pos].key   = key;
    cache[pos].value = result;
    return result;
}

/* public sequence function */
seq_t a(idx_t n) {
    if (n < 2) return 0;          /* a(1) = 0 by definition */

    seq_t res = 1;                /* divisor d = n gives n/d = 1, counted once */

    /* enumerate non‑trivial divisors of n */
    for (idx_t i = 2; (uint64_t)i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* divisor d1 contributes factorization choices of d2 */
            res += factor_count(d2, 2);

            if (d1 != d2) {
                /* divisor d2 contributes factorization choices of d1 */
                res += factor_count(d1, 2);
            }
        }
    }
    return res;
}
