/*
 * A303666  Expansion of 1/((1 - x)*(1 - Sum_{k>=0} x^(2^k))).
 * https://oeis.org/A303666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 38, u64 n <= 77, u128 n <= 155,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A303666: partial sums of binary partition numbers. */
/* Recurrence for p(n):  p(0)=1; for n>0, p(n)=∑_{k:2^k≤n} p(n-2^k). */
/* The desired sequence a(n) is the cumulative sum of the p(n):
   a(0)=1, a(n)=a(n-1)+p(n). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *p = NULL;        /* binary-partition numbers */
    static seq_t *a_seq = NULL;    /* accumulated partial sums */
    static idx_t cap = 0;          /* allocated size */
    static idx_t last = 0;         /* highest index computed */

    /* initialise on first call */
    if (cap == 0) {
        cap = 1;
        p = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        a_seq = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!p || !a_seq) abort();          /* allocation failure */
        p[0] = (seq_t)1;
        a_seq[0] = p[0];
        last = 0;
    }

    /* extend storage if necessary */
    if (n > cap - 1) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp_p = (seq_t *)realloc(p, new_cap * sizeof(seq_t));
        seq_t *tmp_a = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
        if (!tmp_p || !tmp_a) abort();
        p = tmp_p;
        a_seq = tmp_a;
        cap = new_cap;
    }

    /* compute values up to n if we haven't yet */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t pow = 1; pow <= i; pow <<= 1) {
            sum += p[i - pow];
        }
        p[i] = sum;
        a_seq[i] = a_seq[i - 1] + p[i];
    }
    last = n;

    return a_seq[n];
}
