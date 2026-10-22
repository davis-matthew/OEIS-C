/*
 * A327829  Squarefree numbers with a prime number of prime factors.
 * https://oeis.org/A327829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, is_prime_small, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A327829 : squarefree integers whose number of distinct prime factors is a prime.
 * The function returns the n-th term (n ≥ 1) of this sequence.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/is_prime_small.h"

/* a(n) : the n-th squarefree integer with a prime number of distinct prime factors */
seq_t a(idx_t n)
{
    /* ------------- static memoisation state -------------------- */
    static seq_t *cache      = NULL;   /* cached terms */
    static size_t cache_cap = 0;         /* capacity of cache array  */
    static size_t cache_len = 0;         /* number of terms cached  */
    static uint64_t last_checked = 0;    /* last integer examined   */

    /* ------------- build cache until we have n terms ----------- */
    while (cache_len < (size_t)n)
    {
        ++last_checked;
        uint64_t cand = last_checked;

        /* fast square‑free test */
        if (is_squarefree((seq_t)cand))
        {
            /* count distinct prime factors */
            int omega = count_distinct_primes(cand);

            /* check that this count itself is prime (small < 64) */
            if (is_prime_small((seq_t)omega))
            {
                /* grow cache if necessary */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = (cache_cap ? cache_cap * 2 : 8);
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();                /* memory exhausted */
                    cache     = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)cand;
            }
        }
    }

    /* ------------- return cached term -------------------------- */
    return cache[n - 1];
}
