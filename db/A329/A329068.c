/*
 * A329068  a(1) = 1; thereafter if the sum of digits of all previous terms up
 *          to a(n) is even then a(n+1) = (sum of digits of all previous
 *          terms)/2, otherwise a(n+1) = (sum of digits of all previous
 *          terms)*3 + 1.
 * https://oeis.org/A329068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1; for n≥1, let S(n)=Σ_{k=1..n} digit_sum(a(k)), then
 *            a(n+1) = S(n)/2 if S(n) is even, otherwise a(n+1) = S(n)*3 + 1
 * Range:     u8 n <= 15, u16 n <= 1227, u32 n <= 100001, u64 n <= 100001,
 *            u128 n <= 100001, big any n
 * Validated: standard, pass, 73 terms through n = 6945 (2026-11-03)
 * Helpers:   digit_sum, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Static cache for computed terms and their cumulative digit sums */
    static seq_t *terms = NULL;
    static seq_t *cum   = NULL;
    static size_t  sz   = 0;   /* number of terms computed */
    static size_t  cap  = 0;   /* allocated capacity */

    if (n == 0) return 0; /* undefined, but guard */

    /* Initialise cache on first call */
    if (terms == NULL) {
        cap = 16;
        terms = mem_new(seq_t, cap);
        cum   = mem_new(seq_t, cap);
        if (!terms || !cum) return 0;  /* allocation failure */
        /* a(1) = 1  */
        terms[0] = (seq_t)1;
        cum[0]   = digit_sum(terms[0]);   /* sum of digits of a(1) */
        sz       = 1;
    }

    /* Extend cache up to the requested index n */
    while (sz < (size_t)n) {
        seq_t sum_digits = cum[sz - 1];    /* S(sz) */
        seq_t next;

        if ((sum_digits % (seq_t)2) == 0)
            next = sum_digits / (seq_t)2;
        else
            next = sum_digits * (seq_t)3 + (seq_t)1;

        /* Resize arrays if needed */
        if (sz >= cap) {
            size_t new_cap = cap * 2;
            if (new_cap < sz + 1) new_cap = sz + 1;

            seq_t *tmp = mem_resize(seq_t, terms, new_cap);
            if (!tmp) break;
            terms = tmp;

            tmp = mem_resize(seq_t, cum, new_cap);
            if (!tmp) break;
            cum = tmp;

            cap = new_cap;
        }

        terms[sz] = next;
        cum[sz]   = cum[sz - 1] + digit_sum(next);
        sz++;
    }

    return terms[n - 1];
}
