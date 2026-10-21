/*
 * A293835  a(n) = smallest number (in absolute value) not yet in the sequence
 *          such that the arithmetic mean of the first n terms a(1), a(2),
 *          ..., a(n) is an integer; a(1)=1. No two numbers with the same
 *          absolute value may appear. Preference is given to positive values
 *          of a(n).
 * https://oeis.org/A293835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = smallest unused integer (by absolute value) such that the
 *            arithmetic mean of the first n terms is an integer; a(1)=1.
 *            Preference is given to positive values. This is computed by
 *            searching candidates in increasing absolute value
 * Range:     i8 n <= 22, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* malloc, realloc, abort */
#include <string.h>      /* memset */
#include <stdint.h>      /* uint64_t */
#include <stdbool.h>     /* bool */

static seq_t *seq = NULL;          /* array of terms */
static size_t seq_len = 0;         /* number of terms stored */
static size_t seq_cap = 0;         /* allocated capacity for seq */

static seq_t sum = 0;              /* sum of all stored terms */

static unsigned char *used = NULL; /* bitmap of absolute values used */
static size_t used_cap = 0;        /* allocated size of used bitmap */

/* enlarge seq array if needed */
static void ensure_seq(size_t new_len)
{
    if (new_len <= seq_cap)
        return;
    size_t new_cap = (seq_cap ? seq_cap * 2 : 64);
    while (new_cap < new_len)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp)               /* allocation failed */
        abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* enlarge used bitmap if needed */
static void ensure_used(size_t abs)
{
    if (abs < used_cap)
        return;
    size_t new_cap = (used_cap ? used_cap * 2 : 64);
    while (new_cap <= abs)
        new_cap *= 2;
    unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
    if (!tmp)               /* allocation failed */
        abort();
    /* zero the newly allocated part */
    memset(tmp + used_cap, 0, new_cap - used_cap);
    used = tmp;
    used_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Ensure we have at least n terms */
    while (seq_len < (size_t)n) {

        /* First term */
        if (seq_len == 0) {
            ensure_seq(1);
            seq[0] = (seq_t)1;
            seq_len = 1;
            sum = (seq_t)1;
            ensure_used(1);
            used[1] = 1;
            continue;
        }

        /* Index of the term we are about to compute (1‑based) */
        idx_t idx = (idx_t)(seq_len + 1);
        seq_t chosen = 0;           /* term to insert */

        /* Search for the smallest unused absolute value */
        for (idx_t cand = 0;; ++cand) {
            size_t abs = (size_t)cand;
            ensure_used(abs);
            if (used[abs])
                continue;

            seq_t v = (seq_t)cand;          /* positive candidate */

            /* test +v */
            seq_t r1 = (sum + v) % (seq_t)idx;
            if (r1 < 0)
                r1 += (seq_t)idx;
            if (r1 == 0) {
                chosen = v;
                break;
            }

            /* test -v */
            seq_t r2 = (sum - v) % (seq_t)idx;
            if (r2 < 0)
                r2 += (seq_t)idx;
            if (r2 == 0) {
                chosen = -v;
                break;
            }
        }

        /* Store the chosen term */
        ensure_seq(seq_len + 1);
        seq[seq_len++] = chosen;
        sum += chosen;

        /* Mark the absolute value as used */
        size_t abs_chosen = (chosen >= 0) ? (size_t)chosen
                                          : (size_t)(-chosen);
        ensure_used(abs_chosen);
        used[abs_chosen] = 1;
    }

    return seq[(size_t)n - 1];
}
