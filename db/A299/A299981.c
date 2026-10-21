/*
 * A299981  Lexicographic first sequence of positive integers such that
 *          a(n)*a(n+1) has a digit 1, and no term occurs twice.
 * https://oeis.org/A299981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Lexicographic first permutation of positive integers such that
   a(n) * a(n+1) contains a decimal digit '1', each term appears once. */

/* Static cache for generated terms */
static seq_t *seq_cache = NULL;
static size_t  seq_cache_len = 0;     /* number of terms already computed */
static size_t  seq_cache_cap = 0;     /* allocated capacity of seq_cache */

/* Static array marking which positive integers have already been used
   (index 0 is unused). */
static bool *used = NULL;
static size_t used_cap = 0;

/* Ensure seq_cache can hold at least 'n' terms. */
static void ensure_seq_cache(size_t n)
{
    if (seq_cache_len >= n)
        return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap : 256;
    while (new_cap < n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    seq_cache_cap = new_cap;
}

/* Ensure the used array can index at least indices 0 .. needed-1. */
static void ensure_used(size_t needed)
{
    if (needed <= used_cap)
        return;
    size_t new_cap = used_cap ? used_cap : 1024;
    while (new_cap < needed)
        new_cap <<= 1;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (used_cap < new_cap)
        memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = tmp;
    used_cap = new_cap;
}

/* Return true if the decimal representation of x contains digit '1'. */
static inline bool has_digit_one(seq_t x)
{
    if (x == (seq_t)0)
        return false;                /* zero has no digit 1 */
    while (x != (seq_t)0) {
        if ((x % (seq_t)10) == (seq_t)1)
            return true;
        x /= (seq_t)10;
    }
    return false;
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;             /* undefined for n < 1 */

    size_t idx = (size_t)n;          /* indices are 1‑based */

    /* Compute terms up to idx if not already done. */
    if (seq_cache_len < idx) {
        ensure_seq_cache(idx);

        /* Generate missing terms. */
        for (size_t i = seq_cache_len; i < idx; ++i) {
            if (i == 0) {            /* a(1) = 1 */
                seq_cache[0] = (seq_t)1;
                ensure_used(2);      /* indices 0 and 1 exist */
                used[1] = true;
            } else {
                seq_t prev = seq_cache[i - 1];
                seq_t chosen = 0;

                /* Find the smallest unused candidate with the digit‑1 rule. */
                for (seq_t cand = 1; ; ++cand) {
                    size_t cand_idx = (size_t)cand;
                    if (cand_idx + 1 > used_cap)
                        ensure_used(cand_idx + 1);
                    if (!used[cand_idx]) {
                        seq_t prod = prev * cand;
                        if (has_digit_one(prod)) {
                            chosen = cand;
                            break;
                        }
                    }
                }

                seq_cache[i] = chosen;
                ensure_used((size_t)chosen + 1);
                used[(size_t)chosen] = true;
            }
        }

        seq_cache_len = idx;
    }

    return seq_cache[idx - 1];
}
