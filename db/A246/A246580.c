/*
 * A246580  G.f.: x^(k^2)/(mul(1-x^(2*i),i=1..k)*mul(1+x^(2*r-1),r=1..oo))
 *          with k=4.
 * https://oeis.org/A246580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 30, i16 n <= 56, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A246580
 *
 * Generating function:
 *   x^16 / ( (1 - x^2)(1 - x^4)(1 - x^6)(1 - x^8)
 *             * ∏_{r≥1} (1 + x^(2r-1)) )
 *
 * This implementation computes the coefficient of x^n in that series.
 * It uses a straightforward convolution algorithm to combine the
 * series for 1/((1−x^2)(1−x^4)(1−x^6)(1−x^8)) and
 * 1/∏_{odd}(1+x^odd).  The series for the second factor is built
 * by a simple recurrence that directly implements the expansion
 * of each 1/(1+x^odd).
 *
 * The function works for any seq_t (native integer or arbitrary‑precision)
 * and idx_t (unsigned 64‑bit).  It memoises the computed values so
 * that repeated calls with increasing n are O(1) after the first call.
 */

#include <stdlib.h>
#include <string.h>

/* The generated series for 1/((1−x^2)(1−x^4)(1−x^6)(1−x^8)) */
static seq_t *c = NULL;      /* length = c_len   */
static size_t c_len = 0;

/* The generated series for 1/∏_{odd}(1+x^odd) */
static seq_t *d = NULL;      /* length = d_len   */
static size_t d_len = 0;

/* The convolution c * d shifted by 16, stored as a */
static seq_t *a_arr = NULL;  /* length = a_len   */
static size_t a_len = 0;

/* Recompute c and d up to indices 0..need inclusive.
 * If need < c_len (and d_len), nothing is done.
 */
static void recompute_cd(size_t need)
{
    if (need < c_len && need < d_len)
        return;

    /* Allocate new arrays */
    seq_t *c_new = (seq_t *)malloc((need + 1) * sizeof(seq_t));
    seq_t *d_new = (seq_t *)malloc((need + 1) * sizeof(seq_t));
    if (!c_new || !d_new) {
        /* out of memory : abort */
        exit(1);
    }

    /* Compute c : 1/((1−x^2)(1−x^4)(1−x^6)(1−x^8)) */
    memset(c_new, 0, (need + 1) * sizeof(seq_t));
    c_new[0] = (seq_t)1;
    const int parts[4] = {2, 4, 6, 8};
    for (int p = 0; p < 4; ++p) {
        int part = parts[p];
        for (size_t i = part; i <= need; ++i) {
            c_new[i] += c_new[i - part];
        }
    }

    /* Compute d : 1/∏_{odd}(1+x^odd)  by successive multiplication
     * of 1/(1+x^odd):  new[i] = old[i] – new[i–odd].
     */
    memset(d_new, 0, (need + 1) * sizeof(seq_t));
    d_new[0] = (seq_t)1;          /* start with 1 */
    for (size_t odd = 1; odd <= need; odd += 2) {
        for (size_t i = odd; i <= need; ++i) {
            d_new[i] -= d_new[i - odd];
        }
    }

    /* Free old arrays */
    free(c);
    free(d);

    c = c_new;
    d = d_new;
    c_len = d_len = need + 1;
}

/* Ensure a_arr has values up to index n inclusive.
 * The function recomputes c, d and the convolution if needed.
 */
static void ensure_a_len(idx_t n)
{
    if ((size_t)n < a_len)
        return;               /* already computed */

    /* Step 1: compute c and d up to index n-16 (if n≥16) */
    size_t need_cd = (n >= 16) ? (size_t)(n - 16) : 0;
    recompute_cd(need_cd);

    /* Step 2: allocate new a_arr */
    size_t new_len = (size_t)n + 1;
    seq_t *a_new = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!a_new) exit(1);
    memset(a_new, 0, new_len * sizeof(seq_t));

    /* Step 3: convolution c * d shifted by 16 */
    for (size_t i = 0; i < c_len; ++i) {
        for (size_t j = 0; j < d_len; ++j) {
            size_t idx = i + j + 16;
            if (idx > n)
                break;           /* remaining j will only increase idx */
            a_new[idx] += c[i] * d[j];
        }
    }

    /* Step 4: store result */
    free(a_arr);
    a_arr = a_new;
    a_len = new_len;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 16)
        return (seq_t)0;          /* all terms before 16 are zero */

    ensure_a_len(n);
    return a_arr[(size_t)n];
}
