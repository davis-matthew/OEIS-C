/*
 * A256302  Least prime p such that p+3*k*(k+1) is prime for all k=0,...,n.
 * https://oeis.org/A256302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   least prime p such that p + 3*k*(k+1) is prime for all k=0..n
 * Range:     u8 n <= 8, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"  /* provides bool is_prime_big(seq_t n) */

/* Return the next prime >= start */
static seq_t next_prime(seq_t start)
{
    seq_t p = start;
    if (p <= 2)
        p = 2;
    else if ((p & 1) == 0)           /* make p odd if >2 */
        ++p;
    for (;;)
    {
        if (is_prime_big(p))
            return p;
        p += 2;
    }
}

/* Cache for previously computed terms */
static seq_t *cache = NULL;          /* array of a(i) values */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t  last_computed = 0;     /* highest index whose value is cached */
static seq_t  current_prime = 0;     /* minimal prime working up to last_computed */

/* Ensure cache can hold indices up to needed (inclusive) */
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                    /* out of memory */
    /* Initialize new slots to 0 */
    for (size_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = 0;
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    ensure_cache_size(idx + 1);

    /* First call: initialise cache[0] */
    if (cache[0] == 0) {
        cache[0] = 2;
        last_computed   = 0;
        current_prime   = 2;
    }

    /* Return already computed value */
    if (cache[idx] != 0)
        return cache[idx];

    /* Extend cache up to index n */
    for (idx_t m = last_computed + 1; m <= n; ++m) {
        /* value that must be prime for current_prime at k = m */
        seq_t inc = (seq_t)3 * ((seq_t)m * ((seq_t)m + 1));
        seq_t val = current_prime + inc;
        if (!is_prime_big(val)) {
            /* Search next prime that satisfies all k = 0..m */
            seq_t cand = next_prime(current_prime + 1);
            while (1) {
                bool ok = true;
                for (idx_t k = 0; k <= m; ++k) {
                    seq_t inck = (seq_t)3 * ((seq_t)k * ((seq_t)k + 1));
                    seq_t vk   = cand + inck;
                    if (!is_prime_big(vk)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    current_prime = cand;
                    break;
                }
                cand = next_prime(cand + 1);
            }
        }
        /* current_prime now works for all k = 0..m */
        cache[(size_t)m] = current_prime;
        last_computed = m;
    }

    return cache[idx];
}
