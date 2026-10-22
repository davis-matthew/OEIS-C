/*
 * A323049  Numbers that are neither 5-smooth nor a sum of two 5-smooth
 *          numbers.
 * https://oeis.org/A323049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer that is neither a 5‑smooth number
 *            nor the sum of two distinct 5‑smooth numbers
 * Range:     u8 n <= 9, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_five_smooth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --- Dependencies ---------------------------------------------------- */
#include <stdlib.h>   /* malloc, realloc, free */
#include <stdbool.h>  /* bool, true, false */
#include <stddef.h>   /* size_t */
#include "../utils/is_five_smooth.h"

/* --- Dynamic storage for the sequence and for the list of 5‑smooth     */
/*     numbers -------------------------------------------------------- */
static seq_t *seq_vals   = NULL; /* array of terms a(1),a(2),...          */
static size_t seq_len    = 0;     /* number of terms currently stored     */
static size_t seq_cap    = 0;     /* allocated capacity of seq_vals       */

static seq_t *smooth_vals = NULL; /* list of 5‑smooth numbers found so far */
static size_t smooth_len  = 0;     /* number of 5‑smooth numbers stored   */
static size_t smooth_cap  = 0;     /* allocated capacity of smooth_vals   */

static idx_t maxChecked  = 0;     /* largest integer examined so far     */

/* --- Ensure enough capacity in dynamic arrays ------------------------ */
static void ensure_seq_cap(size_t needed)
{
    if (needed > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        if (new_cap < needed) new_cap = needed;
        seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

static void ensure_smooth_cap(size_t needed)
{
    if (needed > smooth_cap) {
        size_t new_cap = smooth_cap ? smooth_cap * 2 : 64;
        if (new_cap < needed) new_cap = needed;
        smooth_vals = (seq_t *)realloc(smooth_vals, new_cap * sizeof(seq_t));
        smooth_cap = new_cap;
    }
}

/* --- Main sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0, but harmless */

    if ((size_t)n <= seq_len)      /* already computed term ? */
        return seq_vals[(size_t)n - 1];

    /* Generate terms until we have at least n of them. */
    idx_t cur = maxChecked + 1;
    for (; seq_len < (size_t)n; ++cur) {
        seq_t value = (seq_t)cur;

        /* If 'value' itself is 5‑smooth, remember it but skip for the
           sequence. */
        bool valSmooth = is_five_smooth(value);
        if (valSmooth) {
            ensure_smooth_cap(smooth_len + 1);
            smooth_vals[smooth_len++] = value;
            continue;
        }

        /* Test whether 'value' can be written as a sum of two 5‑smooth
           numbers seen so far. */
        bool sum_of_two = false;
        for (size_t i = 0; i < smooth_len; ++i) {
            seq_t a = smooth_vals[i];
            if (a > value) break;
            seq_t b = value - a;
            if (is_five_smooth(b)) {
                sum_of_two = true;
                break;
            }
        }

        if (!sum_of_two) {
            /* Value satisfies both conditions; store it. */
            ensure_seq_cap(seq_len + 1);
            seq_vals[seq_len++] = value;
        }
    }

    maxChecked = cur - 1;          /* highest integer examined */
    return seq_vals[(size_t)n - 1];
}
