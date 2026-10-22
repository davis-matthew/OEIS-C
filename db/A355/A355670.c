/*
 * A355670  Numbers k such that A246600(k) < A000005(k).
 * https://oeis.org/A355670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   build_primes, grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes OEIS A355670: the n‑th integer k with
    bitwise OR(k, all divisors of k) > k.
    The function generates qualifying numbers on demand and
    caches them for fast repeated access. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/grow_seq.h"
#include "../utils/is_prime.h"
#include "../utils/build_primes.h"

static bool primes_initialized = false;
static idx_t *primes = NULL;          /* list of primes up to the limit */
static size_t primes_cnt = 0;

/* Returns true if k satisfies the definition of A355670. */
static bool qualifies(idx_t k)
{
    if (k <= 1)          /* 1 never qualifies */
        return false;

    /* Build a (large enough) prime table once. */
    if (!primes_initialized) {
        const idx_t PRIME_LIMIT = 1000000;   /* sufficient for all k up to ~1e12 */
        build_primes(PRIME_LIMIT, &primes, &primes_cnt);
        primes_initialized = true;
    }

    /* Generate all divisors of k.  We use a moderate static array
       – the maximum divisor count for k < 1e12 is well below 4096. */
    idx_t divisors[4096];
    size_t dcnt = 1;
    divisors[0] = 1;

    idx_t rem = k;
    for (size_t pi = 0; pi < primes_cnt && primes[pi] * primes[pi] <= rem; ++pi) {
        if (rem % primes[pi] == 0) {
            idx_t p = primes[pi];
            int exp = 0;
            while (rem % p == 0) {        /* count power of p */
                rem /= p;
                ++exp;
            }
            /* multiply existing divisors by powers of p */
            size_t current = dcnt;
            idx_t mult = 1;
            for (int e = 1; e <= exp; ++e) {
                mult *= p;
                for (size_t j = 0; j < current; ++j)
                    divisors[dcnt++] = divisors[j] * mult;
            }
        }
    }
    if (rem > 1) {   /* remaining prime factor */
        idx_t p = rem;
        size_t current = dcnt;
        idx_t mult = p;     /* p^1 */
        for (size_t j = 0; j < current; ++j)
            divisors[dcnt++] = divisors[j] * mult;
    }

    /* Compute the bitwise OR of k with all its divisors. */
    seq_t or_val = (seq_t)k;
    for (size_t i = 0; i < dcnt; ++i)
        or_val |= (seq_t)divisors[i];

    return or_val > (seq_t)k;
}

/*  External linkage required by the build system.  */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cached sequence values   */
    static size_t cap   = 0;        /* capacity of cache array  */
    static size_t len   = 0;        /* number of values stored  */
    static idx_t  last_k = 1;       /* last integer examined    */

    /* Expand the cache until it contains at least n terms. */
    while (len < n) {
        ++last_k;
        if (qualifies(last_k)) {
            if (len == cap) {
                size_t new_cap = (cap == 0 ? 4 : cap * 2);
                grow_seq(&cache, &cap, new_cap);
            }
            cache[len++] = (seq_t)last_k;
        }
    }
    return cache[n - 1];
}
