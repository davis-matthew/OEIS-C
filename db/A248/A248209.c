/*
 * A248209  Numbers n such that n - A007953(n) contains the same distinct
 *          digits as n.
 * https://oeis.org/A248209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   A248209 – numbers n such that n - digit_sum(n) contains the *
 *            same set of distinct decimal digits as n. Computed by
 *            brute‑force * search; no closed form is known
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   distinct_digit_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/distinct_digit_mask.h"

static inline seq_t digit_sum(seq_t v)
{
    seq_t sum = 0;
    while (v) {
        sum += v % (seq_t)10;
        v /= (seq_t)10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;  /* array of found terms */
    static size_t capacity = 0;
    static size_t used = 0;      /* number of terms stored */
    static seq_t next = 1;       /* next integer to test */

    /* Build terms until we have at least n of them. */
    while (used < n) {
        seq_t cur = next;
        seq_t ds  = digit_sum(cur);
        seq_t diff = cur - ds;
        if (distinct_digit_mask(cur) == distinct_digit_mask(diff)) {
            if (used >= capacity) {
                size_t newcap = capacity ? capacity * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                if (!tmp) exit(1);  /* allocation failure */
                terms = tmp;
                capacity = newcap;
            }
            terms[used++] = cur;
        }
        ++next;  /* advance to next candidate */
    }
    return terms[n - 1];
}
