/*
 * A346308  Intersection of Beatty sequences for sqrt(2) and sqrt(3).
 * https://oeis.org/A346308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   intersection of Beatty sequences for sqrt(2) and sqrt(3). a(n) =
 *            the n‑th positive integer that appears in both sequences B₂ =
 *            ⌊k·√2⌋ and B₃ = ⌊k·√3⌋. The value is obtained by a two‑pointer
 *            traversal of the two Beatty sequences without using
 *            floating‑point arithmetic
 * Range:     u8 n <= 2, u16 n <= 50, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include "../utils/floor_sqrt.h"

/* Helper: compute ⌊k·√α⌋ using integer arithmetic.
   For α = 2 or 3, ⌊k·√α⌋ = ⌊√(α·k²)⌋ and thus can be obtained with floor_sqrt. */
static inline seq_t beatty(idx_t k, int coeff)
{
    /* coeff is 2 or 3.  Cast to seq_t for arithmetic. */
    return floor_sqrt((seq_t)coeff * (seq_t)k * (seq_t)k);
}

/* Cached intersection sequence. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;   /* allocated capacity */
static size_t cache_len  = 0;   /* number of stored terms */

/* Current indices in the Beatty sequences. */
static idx_t i_idx = 1;
static idx_t j_idx = 1;

/* Current values of the two Beatty sequences. */
static seq_t val_i = 0;
static seq_t val_j = 0;

/* Ensure cache is allocated with at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    while (new_cap < need)
        new_cap *= 2;
    void *tmp = realloc((void *)cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache      = (seq_t *)tmp;
    cache_cap  = new_cap;
}

/* One‑time initialization of the generator and cache. */
static void init_sequence(void)
{
    if (cache != NULL) /* already initialized */
        return;
    ensure_cache_cap(64);
    cache_len = 0;
    i_idx = 1;
    j_idx = 1;
    val_i = beatty(i_idx, 2);
    val_j = beatty(j_idx, 3);
}

/* Main function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for n < 1 */
        return 0;

    init_sequence();

    /* Generate terms until we have at least 'n' stored. */
    while (cache_len < n) {
        /* Advance the two pointers until an intersection is found. */
        while (1) {
            if (val_i == val_j) {            /* intersection found */
                ensure_cache_cap(cache_len + 1);
                cache[cache_len++] = val_i;

                /* Move to the next elements in both Beatty sequences. */
                ++i_idx;
                val_i = beatty(i_idx, 2);
                ++j_idx;
                val_j = beatty(j_idx, 3);
                break;                      /* ready for next intersection */
            } else if (val_i < val_j) {       /* B₂ is behind */
                ++i_idx;
                val_i = beatty(i_idx, 2);
            } else {                          /* B₃ is behind */
                ++j_idx;
                val_j = beatty(j_idx, 3);
            }
        }
    }

    return cache[n - 1];
}
