/*
 * A358014  Expansion of e.g.f. 1/(1 - x^3 * (exp(x) - 1)).
 * https://oeis.org/A358014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = n! * sum_{k=4..n} a(n-k)/((k-3)! * (n-k)!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---- static caches for factorials and sequence values ---- */
    static seq_t *fact = NULL;    /* factorials: fact[i] = i!  */
    static idx_t  fact_len = 0;   /* current length (max computed index + 1) */
    static idx_t  fact_cap = 0;   /* allocated capacity */
    static seq_t *seq   = NULL;   /* sequence values a[i] */
    static idx_t  seq_len = 0;   /* current length (max index + 1) */
    static idx_t  seq_cap = 0;   /* allocated capacity */

    /* ---- ensure factorial cache is large enough ---- */
    if (fact_len <= n) {
        idx_t new_cap = fact_cap ? fact_cap * 2 : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_fact = (seq_t*)realloc(fact, (size_t)new_cap * sizeof(seq_t));
        fact = new_fact;
        fact_cap = new_cap;
        /* initialise missing factorials */
        if (fact_len == 0) {
            fact[0] = (seq_t)1;
            fact_len = 1;
        }
        for (idx_t i = fact_len; i <= n; ++i) {
            fact[i] = fact[i-1] * (seq_t)i;
        }
        fact_len = n + 1;
    }

    /* ---- ensure sequence cache is large enough ---- */
    if (seq_len <= n) {
        idx_t new_cap = seq_cap ? seq_cap * 2 : 4; /* start small */
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_seq = (seq_t*)realloc(seq, (size_t)new_cap * sizeof(seq_t));
        seq = new_seq;
        seq_cap = new_cap;
        if (seq_len == 0) {
            seq[0] = (seq_t)1;
            /* a(1),a(2),a(3) = 0 */
            if (seq_cap > 1) seq[1] = (seq_t)0;
            if (seq_cap > 2) seq[2] = (seq_t)0;
            if (seq_cap > 3) seq[3] = (seq_t)0;
            seq_len = 4;
        }
        /* compute new terms up to n */
        for (idx_t i = seq_len; i <= n; ++i) {
            if (i < 4) {
                seq[i] = (seq_t)0;
                continue;
            }
            /* initial product of i*(i-1)*(i-2)*(i-3) */
            seq_t prod = (seq_t)i;
            prod *= (seq_t)(i-1);
            prod *= (seq_t)(i-2);
            prod *= (seq_t)(i-3);
            seq_t sum = (seq_t)0;
            for (idx_t k = 4; k <= i; ++k) {
                seq_t denom = fact[k-3];      /* (k-3)! */
                seq_t factor = prod / denom;  /* integer division, exactly divisible */
                sum += seq[i-k] * factor;
                if (k < i) prod *= (seq_t)(i - k); /* prepare product for next k */
            }
            seq[i] = sum;
        }
        seq_len = n + 1;
    }

    return seq[n];
}
