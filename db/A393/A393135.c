/*
 * A393135  G.f. A(x) satisfies: A(x) = 1 + x * A(-x)^2 / (1 - x).
 * https://oeis.org/A393135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 13, i32 n <= 24, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A393135
   a(0) = 1
   for n > 0: a(n) = Σ_{j=0}^{n-1} Σ_{i=0}^{j} (-1)^j * a(i) * a(j-i)
*/

#include <stdlib.h>
#include <stddef.h>

static seq_t *seq = NULL;          /* memoised values of a(n)          */
static seq_t *conv_arr = NULL;     /* convolution values             */
static size_t allocated = 0;       /* allocated size of the arrays    */
static idx_t  max_index = 0;       /* last index for which a was computed */

/* ensure arrays are large enough to hold indices up to 'needed' */
static void ensure_capacity(size_t needed)
{
    if (needed <= allocated)
        return;
    size_t new_cap = allocated ? allocated << 1 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq      = (seq_t *)realloc(seq,      new_cap * sizeof(seq_t));
    conv_arr = (seq_t *)realloc(conv_arr, new_cap * sizeof(seq_t));
    allocated = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise on first call */
    if (seq == NULL) {
        ensure_capacity(2);          /* need at least space for indices 0 and 1 */
        seq[0]  = (seq_t)1;
        conv_arr[0] = seq[0] * seq[0];  /* conv[0] = a0*a0 = 1 */
        max_index = 0;
    }

    /* grow if necessary */
    if ((size_t)n + 1 > allocated)
        ensure_capacity((size_t)n + 1);

    /* compute values up to n if not already done */
    for (idx_t cur = max_index + 1; cur <= n; ++cur) {
        if (cur == 1) {
            /* a(1) = conv[0] */
            seq[1] = conv_arr[0];
        } else {
            /* a(cur) = a(cur-1) + (-1)^(cur-1) * conv[cur-1] */
            seq_t add = ((cur-1) & 1) ? -conv_arr[cur-1] : conv_arr[cur-1];
            seq[cur] = seq[cur-1] + add;
        }

        /* compute convolution conv[cur] = Σ_{i=0}^{cur} a(i)*a(cur-i) */
        seq_t sum = 0;
        for (idx_t i = 0; i <= cur; ++i) {
            sum += seq[i] * seq[cur - i];
        }
        conv_arr[cur] = sum;
    }

    max_index = n;
    return seq[n];
}
