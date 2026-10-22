/*
 * A345475  Nonsquares k whose continued fraction for the square root of k has
 *          a periodic part that is a nondecreasing sequence.
 * https://oeis.org/A345475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A345475 – nonsquares k whose continued‑fraction of √k
   has a periodic part that is nondecreasing.  */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Cache of computed terms (0‑based). */
static seq_t *cache   = NULL;     /* array of seq_t */
static idx_t  cache_cnt = 0;      /* number of elements stored */
static idx_t  cache_cap = 0;      /* capacity of the array */
static seq_t next_k   = 2;        /* next integer to test */

/* Ensure the cache has room for at least 'needed' elements. */
static void ensure_cache_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2u : 4u;
    while (new_cap < needed)
        new_cap <<= 1u;

    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp) {
        abort();  /* allocation failure */
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Return true iff k is a nonsquare and the periodic part of sqrt(k)'s
   continued fraction is nondecreasing. */
static bool is_good_k(seq_t k)
{
    seq_t a0 = floor_sqrt(k);
    if (a0 * a0 == k)            /* perfect square */
        return false;

    seq_t m = 0;                 /* m_n in standard notation */
    seq_t d = 1;                 /* d_n */
    seq_t a = a0;                /* a_n, starts with a0 */

    seq_t prev = 0;
    bool first = true;

    while (1) {
        /* Next values in the continued‑fraction iteration. */
        seq_t m_next = d * a - m;
        seq_t d_next = (k - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* The period ends when the next term equals 2*a0. */
        if (a_next == a0 * 2u)
            break;

        /* Check the nondecreasing condition. */
        if (first) {
            first = false;
            prev = a_next;
        } else {
            if (a_next < prev)
                return false;
            prev = a_next;
        }

        /* Advance to the next step. */
        m = m_next;
        d = d_next;
        a = a_next;
    }
    return true;
}

/* Return the n‑th term of sequence A345475 (1‑based). */
seq_t a(idx_t n)
{
    while (cache_cnt < n) {
        if (is_good_k(next_k)) {
            ensure_cache_capacity(cache_cnt + 1);
            cache[cache_cnt] = next_k;
            cache_cnt++;
        }
        next_k++;
    }
    return cache[n - 1];
}
