/*
 * A272464  Fractal sequence related to Stern's diatomic sequence (A002487).
 * https://oeis.org/A272464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 332, u16 n <= 8204, u32 n <= 8204, u64 n <= 8204,
 *            u128 n <= 8204, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A272464 – Fractal sequence related to Stern’s diatomic sequence.
 *  a(1)=a(2)=1.
 *  For n≥3 let k be the largest index < n with a(k)=a(n‑1).
 *  If a(k)+a(k+1) has not appeared earlier in the sequence
 *      a(n)   = a(k)+a(k+1)        (new term)
 *      a(n+1) = a(k+1)             (second new term)
 *  otherwise
 *      a(n)   = a(k+1)             (only one new term)
 *  The function memoises the sequence so that any call to a(n)
 *  (in any order) runs in O(n²) in the worst case, which is well
 *  below the limits for n up to about 10000.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *seq = NULL;          /* 1‑based sequence array (seq[0] unused) */
static size_t seq_cap = 0;         /* allocated capacity of seq array */
static idx_t  seq_len = 0;         /* number of terms currently stored */

/* Grow the sequence array to hold at least 'need' terms (1‑based).   */
static void
seq_grow(idx_t need)
{
    if (seq_len >= need) return;
    size_t new_cap = seq_cap ? seq_cap : 256;
    while (new_cap < (size_t)need) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, (new_cap + 1) * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Find the previous occurrence of value 'v' before the current
 * longest index.  Returns 0 if it has not appeared before
 * (which should not happen for n≥3). */
static idx_t
prev_occurrence(seq_t v)
{
    /* scan backwards from the current length-1 */
    for (size_t i = seq_len - 1; i > 0; --i) {
        if (seq[i] == v) return (idx_t)i;
    }
    return 0;   /* safety fallback – should not be used */
}

/* Return the largest index < n where a(index)=val. */
static idx_t
last_occurrence_before(seq_t val, idx_t limit)
{
    for (size_t i = limit - 1; i > 0; --i) {
        if (seq[i] == val) return (idx_t)i;
    }
    return 0;
}

/* Main interface: return a(n) for any n ≥ 1.  The function is
 * memoised, so later calls reuse the generated terms. */
seq_t a(idx_t n)
{
    /* lazy initialisation of the array */
    if (seq == NULL) {
        seq_cap = 256;
        seq     = (seq_t *)calloc(seq_cap + 1, sizeof(seq_t));  /* seq[0] unused */
        seq_len = 2;
        seq[1] = seq[2] = (seq_t)1;
    }

    if (n <= seq_len) return seq[n];

    /* Generate sequence up to required index */
    while (seq_len < n) {
        /* Current last term */
        idx_t last = seq_len;
        seq_t val  = seq[last];

        /* Find previous occurrence of 'val' */
        idx_t k = prev_occurrence(val);
        if (k == 0 && val == (seq_t)1) k = 1;  /* safety for the initial 1 */

        seq_t a_k   = seq[k];
        seq_t a_k1  = seq[k + 1];
        seq_t sum   = a_k + a_k1;

        /* Check if 'sum' has appeared before last */
        idx_t sum_last = last_occurrence_before(sum, last);

        if (sum_last == 0) {
            /* Append sum */
            seq_grow(seq_len + 1);
            seq[++seq_len] = sum;

            /* Append a_k1 */
            seq_grow(seq_len + 1);
            seq[++seq_len] = a_k1;
        } else {
            /* Append only a_k1 */
            seq_grow(seq_len + 1);
            seq[++seq_len] = a_k1;
        }
    }

    return seq[n];
}
