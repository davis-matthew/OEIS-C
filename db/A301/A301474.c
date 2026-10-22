/*
 * A301474  Summarize the double of the previous term (digits in increasing
 *          order), starting with a(1) = 1.
 * https://oeis.org/A301474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A301474:
   a(1) = 1.
   For n > 1 let x = 2*a(n‑1).  For each decimal digit d (0–9)
   let c_d be the number of times d occurs in the decimal
   representation of x.  Then a(n) is the concatenation, in
   increasing order of d, of decimal representations of c_d
   followed by d.  Example: a(1)=1 → 2 → "one 2" → 12. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Memoisation cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* currently allocated length */
    static size_t last_computed = 0;  /* maximum index already filled */

    /* Grow cache if necessary */
    if (cap < (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise first term if not done yet */
    if (last_computed == 0) {
        cache[1] = (seq_t)1;
        last_computed = 1;
    }

    /* Compute terms up to n */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        seq_t prev    = cache[i - 1];
        seq_t doubled = prev * 2;

        /* Count digits in doubled */
        seq_t temp = doubled;
        seq_t cnt[10] = {0,0,0,0,0,0,0,0,0,0};
        while (temp > 0) {
            int d = (int)(temp % 10);
            ++cnt[d];
            temp = temp / 10;
        }

        /* Build next term from counts */
        seq_t next = 0;
        for (int d = 0; d <= 9; ++d) {
            if (cnt[d] > 0) {
                /* Append decimal digits of cnt[d] */
                seq_t c = cnt[d];
                int buf[128];
                int len = 0;
                while (c > 0) {
                    buf[len++] = (int)(c % 10);
                    c = c / 10;
                }
                for (int j = len - 1; j >= 0; --j) {
                    next = next * 10 + (seq_t)buf[j];
                }
                /* Append the digit itself */
                next = next * 10 + (seq_t)d;
            }
        }
        cache[i] = next;
    }
    last_computed = (size_t)n;
    return cache[(size_t)n];
}
