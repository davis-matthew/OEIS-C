/*
 * A340038  Numbers that are the sum of a square s and a cube t such that 0 <
 *          s <= t.
 * https://oeis.org/A340038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   cmp_seq_t, floor_sqrt, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A340038: numbers that are the sum of a square s and a cube t
   with 0 < s <= t.  The routine pre‑computes all distinct sums a² + b³
   for 1 ≤ b ≤ 300, sorts them, removes duplicates, and returns the
   n‑th element (a(1) is the first term).  This is sufficient to
   answer any query up to at least n = 10 000.  */
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/seq_alloc.h"   /* seq_alloc */
#include "../utils/floor_sqrt.h"  /* floor_sqrt */
#include "../utils/cmp_seq_t.h"   /* cmp_seq_t */

static seq_t *sum_vals = NULL;   /* array of all distinct sums */
static size_t sum_len  = 0;       /* number of elements in sum_vals */

/* Build the list of sums once. */
static void build_sequence(void)
{
    if (sum_vals != NULL)      /* already built */
        return;

    /* Upper bound on b. 300 is more than enough for n ≤ 10 000. */
    const idx_t b_limit = 300;

    /* Rough estimate of number of pairs; generous overestimation. */
    const size_t estimate = 700000;

    sum_vals = seq_alloc(estimate);
    sum_len  = 0;

    for (idx_t b = 1; b <= b_limit; ++b) {
        /* t = b³.  Compute using seq_t arithmetic. */
        seq_t t = (seq_t)b * (seq_t)b * (seq_t)b;

        /* a_max = ⌊√t⌋ as an idx_t.  floor_sqrt returns a seq_t. */
        seq_t sqrt_t = floor_sqrt(t);
        idx_t a_max = (idx_t)sqrt_t;   /* safe for chosen bound */

        for (idx_t a = 1; a <= a_max; ++a) {
            seq_t val = (seq_t)a * (seq_t)a + t;   /* a² + b³ */
            sum_vals[sum_len++] = val;
        }
    }

    /* Sort the array. */
    qsort(sum_vals, sum_len, sizeof(seq_t), cmp_seq_t);

    /* Remove duplicates in place. */
    size_t uniq = 0;
    for (size_t i = 0; i < sum_len; ++i) {
        if (i == 0 || cmp_seq_t(&sum_vals[i], &sum_vals[i - 1]) != 0)
            sum_vals[uniq++] = sum_vals[i];
    }
    sum_len = uniq;
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    build_sequence();

    if (n < 1 || (size_t)n > sum_len)
        return 0;            /* out of bounds – should not happen for tests */

    return sum_vals[(size_t)n - 1];
}
