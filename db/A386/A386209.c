/*
 * A386209  G.f. A(x) satisfies A(x) = 1/(1-x)^2 + x^2 * (d/dx A(x)^2).
 * https://oeis.org/A386209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 1 + (n-1) * ∑_{k=0}^{n-1} a(k)*a(n-1-k) (OEIS
 *            A386209)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses an incremental convolution table s(m)=∑_{k=0}^m a(k)*a(m-k);
   a(n) = n+1 + (n-1)*s(n-1).  The function is memoised so that any order of calls works
   and each term up to n is computed only once. */

#include <stdlib.h>
#include <stdint.h>

/* static state */
static seq_t *a_tbl = NULL;   /* a values */
static seq_t *s_tbl = NULL;   /* convolution sums */
static size_t tbl_cap = 0;
static size_t tbl_len = 0;

/* ensure capacity: new_cap >= needed+1 */
static void ensure_capacity(size_t needed)
{
    if (tbl_cap > needed) return;
    size_t new_cap = tbl_cap ? tbl_cap : 1;
    while (new_cap <= needed) new_cap <<= 1;
    a_tbl = (seq_t*)realloc(a_tbl, new_cap * sizeof(seq_t));
    s_tbl = (seq_t*)realloc(s_tbl, new_cap * sizeof(seq_t));
    tbl_cap = new_cap;
}

/* public sequence function */
seq_t a(idx_t n)
{
    /* initialise on first use */
    if (tbl_len == 0) {
        ensure_capacity(0);
        a_tbl[0] = (seq_t)1;
        s_tbl[0] = a_tbl[0] * a_tbl[0];  /* s(0)=a0*a0 */
        tbl_len = 1;
    }

    size_t target = (size_t)n;
    if (target >= tbl_len) {
        ensure_capacity(target);
        for (size_t i = tbl_len; i <= target; ++i) {
            /* a(i) */
            seq_t ai = (seq_t)i + 1;  /* i+1 */
            if (i >= 1) {
                ai += (seq_t)(i - 1) * s_tbl[i - 1];
            }
            a_tbl[i] = ai;

            /* compute s(i) */
            seq_t sum = 0;
            for (size_t k = 0; k <= i; ++k) {
                sum += a_tbl[k] * a_tbl[i - k];
            }
            s_tbl[i] = sum;
        }
        tbl_len = target + 1;
    }
    return a_tbl[n];
}
