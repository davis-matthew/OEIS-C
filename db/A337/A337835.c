/*
 * A337835  a(1) = 0 and for n > 1, a(n+1) = k - a(n) where k is the number of
 *          terms equal to a(n) among the first n terms.
 * https://oeis.org/A337835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0; for n>1, a(n)=k-a(n-1) where k is the number of
   terms equal to a(n-1) among the first n-1 terms. */

#include <stdlib.h>

static seq_t *buf = NULL;    /* table of terms, 1‑based index */
static size_t buf_cap = 0;   /* allocated capacity (≥ current_max+1) */
static idx_t cur_max = 0;    /* number of terms actually computed */

/* Ensure that buf[1..n] are computed. */
static void ensure_terms(idx_t n)
{
    if (n <= cur_max) return;               /* already available */

    /* Allocate more space if necessary */
    size_t need = (size_t)(n + 1);
    if (need > buf_cap) {
        void *p = realloc(buf, need * sizeof(seq_t));
        if (!p) abort();                   /* out of memory */
        buf = (seq_t *)p;
        buf_cap = need;
    }

    /* First term initialization */
    if (cur_max == 0) {
        buf[1] = (seq_t)0;
        cur_max = 1;
    }

    /* Build terms cur_max+1 … n */
    for (idx_t i = cur_max + 1; i <= n; ++i) {
        seq_t prev = buf[i - 1];
        idx_t k = 0;
        for (idx_t j = 1; j < i; ++j) {
            if (buf[j] == prev) ++k;
        }
        /* Compute next term using a wider type to avoid overflow */
        long long diff = (long long)k - (long long)prev;
        buf[i] = (seq_t)diff;
    }

    cur_max = n;
}

/* Public function requested by the OJ. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, safe return */
    ensure_terms(n);
    return buf[n];
}
