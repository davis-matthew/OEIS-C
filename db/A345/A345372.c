/*
 * A345372  a(n) = Sum_{i=1..n} nac(i,n) where nac(i,n) is the n-th i-bonacci
 *          number. The n-th i-bonacci number here is equal to 1 for the first
 *          i terms, with subsequent terms equaling the sum of the previous n
 *          terms.
 * https://oeis.org/A345372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} nac(i,n) where nac(i,n) is the n-th
 *            i‑bonacci number
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t buf_static[10001];          /* buffer for n ≤ 10000            */

seq_t a(idx_t n)
{
    /* Handle n < 1: undefined by OEIS, return 0.         */
    if (n == 0) return 0;

    /* Decide whether to use the static buffer or allocate. */
    size_t buf_len = (size_t)n;
    seq_t *buf;
    if (buf_len <= 10000) {
        buf = buf_static;
    } else {
        buf = (seq_t*)malloc(buf_len * sizeof(seq_t));
        if (!buf) return 0;        /* allocation failed – return 0     */
    }

    seq_t total = 0;                /* a(n) */
    for (idx_t i = 1; i <= n; ++i) {
        if (i >= n) {               /* nac(i,n) = 1 for i ≥ n           */
            total += 1;
            continue;
        }

        /* Build the i‑bonacci sequence up to position n.   */
        size_t len   = (size_t)i;          /* window size                   */
        size_t steps = buf_len - len;      /* number of updates needed      */

        /* Initialise the first i terms to 1.                */
        for (size_t j = 0; j < len; ++j) {
            buf[j] = 1;
        }

        seq_t sum  = (seq_t)len;    /* current sum of window          */
        size_t idx = 0;             /* position to overwrite          */
        seq_t term = 0;             /* will hold the i‑bonacci term   */

        for (size_t s = 0; s < steps; ++s) {
            term = sum;             /* new term is the sum of the window */
            seq_t old = buf[idx];   /* element leaving the window        */
            buf[idx] = term;        /* insert new term                   */
            sum += term;            /* update window sum                 */
            sum -= old;             /* subtract old term                 */
            ++idx;
            if (idx == len) idx = 0;
        }

        total += term;              /* add this i‑bonacci value to a(n)   */
    }

    if (buf != buf_static) free(buf);   /* release dynamic allocation     */
    return total;
}
