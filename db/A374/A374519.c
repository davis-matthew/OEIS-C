/*
 * A374519  Numbers k such that the leaders of anti-runs in the k-th
 *          composition in standard order (A066099) are identical.
 * https://oeis.org/A374519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) = the n-th integer k (1‑based) for which the leaders of all
 * anti‑runs in the standard composition of k are identical.
 * The standard composition of k is obtained from the binary
 * representation of k as described in OEIS A066099.
 *
 * The implementation follows the definition directly and memoises
 * the sequence.  It works with any integer type seq_t (built‑in or
 * arbitrary‑precision) and index type idx_t (uint64_t).
 */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Auxiliary test: does a given k satisfy the condition?              */
/* ------------------------------------------------------------------ */
static bool is_good(idx_t k)
{
    /* k == 0: empty composition – trivially satisfies the condition */
    if (k == 0) return true;

    /* positions of 1-bits in k, counting from LSB as position 1 */
    idx_t pos[65];
    unsigned pos_cnt = 0;
    idx_t bit = 1;
    idx_t t = k;
    while (t != 0) {
        if (t & 1) {
            pos[pos_cnt++] = bit;
        }
        ++bit;
        t >>= 1;
    }

    /* differences d_i = pos_i - pos_{i-1} (pos_0 = 0) */
    idx_t d[65];
    unsigned d_cnt = 0;
    idx_t prev = 0;
    for (unsigned i = 0; i < pos_cnt; ++i) {
        d[d_cnt++] = pos[i] - prev;
        prev = pos[i];
    }

    if (d_cnt == 0) return true;          /* empty composition */

    idx_t last = d[d_cnt - 1];
    for (idx_t i = 1; i < d_cnt; ++i) {
        if (d[i] == d[i - 1] && d[i] != last) {
            return false;                 /* different leaders */
        }
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Memoisation state                                                 */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;   /* array of computed values */
static size_t cap   = 0;      /* capacity of the array          */
static size_t size  = 0;      /* number of stored values        */
static idx_t  need  = 0;      /* next k to evaluate             */

/* ------------------------------------------------------------------ */
/* Ensure the cache has at least 'new_cap' slots                      */
/* ------------------------------------------------------------------ */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cap) return;

    size_t newsize = new_cap * 2;
    if (newsize < new_cap) newsize = new_cap;   /* guard against overflow */

    seq_t *newptr = NULL;

#ifdef __cplusplus
    newptr = new seq_t[newsize];
    if (cache) {
        /* copy old contents */
        for (size_t i = 0; i < size; ++i) {
            newptr[i] = cache[i];
        }
        delete[] cache;
    }
#else
    newptr = (seq_t *)realloc(cache, sizeof(seq_t) * newsize);
#endif

    cache = newptr;
    cap   = newsize;
}

/* ------------------------------------------------------------------ */
/* Sequence function                                                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defensive, should not happen */

    /* extend cache until we have 'n' terms */
    while (size < n) {
        if (is_good(need)) {
            ensure_capacity(size + 1);
            cache[size++] = (seq_t)need;
        }
        ++need;
    }
    return cache[n - 1];
}
