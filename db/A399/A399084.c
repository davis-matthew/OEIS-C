/*
 * A399084  a(0) = 0; a(1) = 1; thereafter a(n) = a(n-1) - 1 if that is not
 *          already in the sequence, otherwise a(n) = a(n-1) +
 *          floor(sqrt(a(n-1))).
 * https://oeis.org/A399084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1; for n≥2
   a(n)=a(n−1)−1 if that value has not appeared yet;
   otherwise a(n)=a(n−1)+⌊√(a(n−1))⌋. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static size_t seq_len  = 0;      /* number of terms already computed */
static seq_t *seq_ptr  = NULL;   /* array of computed terms */
static size_t seq_cap  = 0;      /* capacity of seq_ptr */

/* Ensure seq_ptr can hold at least `needed` elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_ptr = (seq_t *)realloc(seq_ptr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Linear search: return true if `value` is present in seq_ptr[0..len-1]. */
static bool exists_in_seq(const seq_t *seq, size_t len, const seq_t value)
{
    for (size_t i = 0; i < len; ++i)
        if (seq[i] == value)
            return true;
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Initialize on first call. */
    if (seq_len == 0) {
        ensure_capacity(2);
        seq_ptr[0] = (seq_t)0;
        seq_ptr[1] = (seq_t)1;
        seq_len   = 2;
    }

    /* Grow the sequence up to index n. */
    while (seq_len <= (size_t)n) {
        seq_t prev = seq_ptr[seq_len - 1];
        seq_t cand = prev - (seq_t)1;                 /* try prev-1 */

        if (exists_in_seq(seq_ptr, seq_len, cand)) {  /* already seen */
            cand = prev + floor_sqrt(prev);
        }

        ensure_capacity(seq_len + 1);
        seq_ptr[seq_len] = cand;
        ++seq_len;
    }

    return seq_ptr[(size_t)n];
}
