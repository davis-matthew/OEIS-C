/*
 * A349872  a(1) = 2; for n > 1, a(n) is the smallest unused number > 1 such
 *          that none of the previous a(n) terms divide a(n).
 * https://oeis.org/A349872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 2;
 * for n > 1, a(n) is the smallest unused number > 1 such that none of the
 * previous a(n) terms divide it.
 * The implementation keeps a static cache of the sequence values and
 * fills missing terms on demand.  Each term is computed by trying
 * increasing candidates, skipping already‑used values, and checking
 * divisibility against the last ‘candidate’ terms of the sequence.
 * The algorithm is O(n²) in the worst case but runs well under the
 * required time limits for n up to about 10 000.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache for sequence values */
static seq_t *seq_cache = NULL;   /* stored terms */
static idx_t  seq_len   = 0;      /* current number of elements stored */
static idx_t  seq_cap   = 0;      /* allocated capacity of seq_cache */

/* Ensure that seq_cache can hold at least 'need' elements */
static void ensure_cache_cap(idx_t need)
{
    if (need <= seq_cap)
        return;
    idx_t new_cap = seq_cap ? seq_cap << 1 : 1;
    while (new_cap < need)
        new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* Test whether 'cand' has already been used among the first 'up_to' terms */
static bool used(seq_t cand, idx_t up_to)
{
    for (idx_t i = 0; i < up_to; ++i) {
        if (seq_cache[i] == cand)
            return true;
    }
    return false;
}

/* Compute the next sequence term (the term at index seq_len+1) */
static seq_t compute_next(void)
{
    /* seq_len holds the number of already computed terms. */
    seq_t cand = 2;
    for (;; ++cand) {
        /* Skip values that have already appeared. */
        if (used(cand, seq_len))
            continue;

        /* Determine the range of the last 'cand' terms to check against.
           If cand > seq_len, we need to test all terms from the beginning. */
        idx_t start = (cand <= (seq_t)seq_len) ? (seq_len - (idx_t)cand) : 0;

        bool ok = true;
        for (idx_t j = start; j < seq_len; ++j) {
            if (cand % seq_cache[j] == 0) {
                ok = false;
                break;
            }
        }
        if (ok)
            return cand;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1u)
        return 0;                /* undefined for n < 1 */

    /* Fill the cache up to index n */
    if (seq_len < n) {
        ensure_cache_cap(n);
        while (seq_len < n) {
            seq_cache[seq_len] = compute_next();
            ++seq_len;
        }
    }
    return seq_cache[n - 1];
}
