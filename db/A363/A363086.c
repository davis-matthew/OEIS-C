/*
 * A363086  a(0)=a(1)=1. For n>1, let c=count of all occurrences of a(n-1) in
 *          the list so far. If c < abs(a(n-1)), then a(n)=c-a(n-1).
 *          Otherwise, a(n)=c.
 * https://oeis.org/A363086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, look
 * Formula:   a(0) = a(1) = 1; For n>1, let c = count of all occurrences of
 *            a(n-1) in the list so far. If c < |a(n-1)| then a(n) = c -
 *            a(n-1); otherwise a(n) = c
 * Range:     i8 n <= 20167, i16 n <= 50000, i32 n <= 50000, i64 n <= 50000,
 *            i128 n <= 50000, big any n
 * Validated: standard, pass, 94 terms through n = 9841 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc, size_t */
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static memoisation of the sequence */
    static seq_t *seq = NULL;
    static size_t len = 0;      /* number of elements computed so far */
    static size_t cap = 0;      /* allocated capacity */

    if (n >= len) {
        /* ensure capacity */
        size_t needed = n + 1;
        if (cap < needed) {
            size_t newcap = cap ? cap * 2 : 16;
            while (newcap < needed) newcap *= 2;
            seq = mem_resize(seq_t, seq, newcap);
            cap = newcap;
        }
        /* compute missing terms */
        while (len <= n) {
            if (len == 0 || len == 1) {
                seq[len] = (seq_t)1;
            } else {
                seq_t prev = seq[len - 1];
                int c = 0;
                for (size_t i = 0; i < len; ++i)
                    if (seq[i] == prev)
                        ++c;
                seq_t abs_prev = (prev < 0) ? -prev : prev;
                seq_t next;
                if ((seq_t)c < abs_prev)
                    next = (seq_t)c - prev;
                else
                    next = (seq_t)c;
                seq[len] = next;
            }
            ++len;
        }
    }
    return seq[n];
}
