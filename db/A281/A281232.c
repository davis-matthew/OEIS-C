/*
 * A281232  Numbers k such that k+2 divides concat(k, k+1).
 * https://oeis.org/A281232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A281232: Numbers k such that k+2 divides concat(k, k+1).
   The algorithm generates terms by iterating over digit lengths d.
   For each d it computes N = 2*10^d + 1 and tests all small
   divisors t = 2,…,20.  If N % t == 0 and m = N/t lies in
   [10^(d-1), 10^d) then k = m-2 is a valid term.  All found k's
   are stored in ascending order; the function returns the n-th
   element (offset 1).  The algorithm uses only integer arithmetic
   and works for any seq_t (built‑in or arbitrary‑precision) and
   any idx_t.  */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static data for lazy, incremental generation */
    static seq_t *table   = NULL;   /* array of computed terms */
    static size_t  sz      = 0;      /* number of stored terms   */
    static size_t  cap     = 0;      /* capacity of 'table'       */
    static seq_t  pow10_prev  = 1;   /* 10^(d-1)                  */

    if (n == 0) return 0; /* undefined offset – return 0 */

    while (sz < (size_t)n) {
        /* compute 10^d safely */
        seq_t pow10_d = pow10_prev * (seq_t)10;
        /* detect overflow for built‑in types */
        if (pow10_d < pow10_prev) break;   /* further terms too large for this build */

        /* N = 2*10^d + 1 */
        seq_t N = pow10_d * (seq_t)2 + (seq_t)1;

        /* collect candidate m values in a small buffer */
        seq_t  cand[21];
        size_t cnt = 0;
        for (idx_t t = 2; t <= 20; ++t) {
            if (N % (seq_t)t == (seq_t)0) {
                seq_t m = N / (seq_t)t;
                if (m >= pow10_prev && m < pow10_d && m >= (seq_t)3) {
                    cand[cnt++] = m;               /* m = k+2 */
                }
            }
        }

        /* sort the few candidates so that k's are appended in ascending order */
        for (size_t i = 1; i < cnt; ++i) {
            seq_t key = cand[i];
            size_t j = i;
            while (j > 0 && cand[j-1] > key) {
                cand[j] = cand[j-1];
                --j;
            }
            cand[j] = key;
        }

        /* ensure enough capacity in the static table */
        size_t needed = sz + cnt;
        if (needed > cap) {
            size_t new_cap = cap ? cap * 2 : 16;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(table, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failed – abort gracefully */
                return (seq_t)0;
            }
            table = tmp;
            cap   = new_cap;
        }

        /* append the new terms (k = m-2) */
        for (size_t i = 0; i < cnt; ++i) {
            table[sz++] = cand[i] - (seq_t)2;
        }

        /* advance to the next digit length */
        pow10_prev = pow10_d;
    }

    /* return the requested term (offset 1) */
    if ((size_t)n > sz) return 0; /* should not happen */
    return table[n-1];
}
