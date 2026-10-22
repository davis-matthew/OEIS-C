/*
 * A306330  Squarefree n with >= 3 factors that admit idempotent
 *          factorizations n = p*q.
 * https://oeis.org/A306330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 5920, u32 n <= 5920, u64 n <= 5920,
 *            u128 n <= 5920, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A306330 – squarefree n with ≥3 prime factors that admit an idempotent
   factorization n = p*q  (i.e. λ(n) | (p−1)(q−1))

   The function builds the sequence incrementally: it scans integers
   upward, checks squarefree‑ness, the number of distinct primes, then
   tests all nontrivial factorizations.  Results are cached so that
   a(n) can be requested in any order.  This satisfies the
   performance constraints for n up to about 10 000. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

static idx_t   max_checked = 0;     /* largest integer already examined   */
static size_t  cache_cap   = 0;     /* capacity of the cache array        */
static size_t  cache_len   = 0;     /* number of elements currently stored*/
static seq_t  *cache       = NULL;  /* first elements of the sequence     */

/* Ensure that @cache has at least @need capacity. */
static void
ensure_cache_capacity(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Least common multiple of a and b, both seq_t.  Assumes a,b≥1. */
static inline seq_t
lcm_seq_t(seq_t a, seq_t b)
{
    seq_t g = gcd(a, b);
    return (a / g) * b;
}

/* Test whether m (uint64_t) is a term of the sequence.  Returns true
   iff m is squarefree, has at least 3 distinct prime factors and
   admits an idempotent factorization. */
static bool
term_candidate(uint64_t m)
{
    /* Factor m.  Use arrays big enough for any needed size.  */
    uint64_t primes[64], exps[64];
    size_t   cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    /* Need at least 3 prime factors. */
    if (cnt < 3) return false;

    /* Must be squarefree. */
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] > 1) return false;

    /* λ(m) = lcm of (p-1) over all prime factors. */
    seq_t lam = 1;
    for (size_t i = 0; i < cnt; ++i)
        lam = lcm_seq_t(lam, (seq_t)(primes[i] - 1));

    /* Test all complementary factorizations.  Exclude trivial p=1 or q=1. */
    const size_t total = 1ULL << cnt;
    for (size_t mask = 1; mask < (total - 1); ++mask) {
        seq_t p = 1;
        for (size_t i = 0; i < cnt; ++i)
            if (mask & (1ULL << i))
                p *= (seq_t)primes[i];
        seq_t q = (seq_t)m / p;
        seq_t prod = (p - 1) * (q - 1);
        if (prod % lam == 0)
            return true;
    }
    return false;
}

/* Return the n-th element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1 */

    /* Grow the cache until we have at least n elements. */
    while (cache_len < (size_t)n) {
        ++max_checked;
        if (term_candidate(max_checked)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)max_checked;
        }
    }
    return cache[n - 1];
}
