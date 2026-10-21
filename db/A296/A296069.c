/*
 * A296069  a(1)=0; thereafter a(n) is the smallest number (in absolute value)
 *          not yet in the sequence such that the arithmetic mean of the first
 *          n terms a(1), a(2), ..., a(n) is a nonzero integer. Preference is
 *          given to positive values of a(n).
 * https://oeis.org/A296069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 127, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A296069.
 * Recurrence: a(1)=0, a(2)=2, a(3)=1, a(4)=5, a(5)=-3, a(6)=7,
 *              a(n) = -a(n-1) + a(n-2) + a(n-3)  for n>6
 */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n<1, return 0 */

    /* static cache for already-computed terms */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t max_computed = 0;

    /* ensure capacity at least 7 (indices 1..6) */
    if (capacity < 7) {
        size_t new_cap = (capacity > 0 ? capacity * 2 : 32);
        if (new_cap < 7) new_cap = 7;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* out of memory, return 0 */
        cache = tmp;
        capacity = new_cap;
    }

    /* initialize first six terms if not yet done */
    if (max_computed < 6) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)-3;
        cache[6] = (seq_t)7;
        max_computed = 6;
    }

    /* compute up to n if needed */
    size_t need = (size_t)n + 1;       /* index 0 unused */
    if (capacity < need) {
        size_t new_cap = capacity * 2;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* out of memory */
        cache = tmp;
        capacity = new_cap;
    }

    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[i] = -cache[i-1] + cache[i-2] + cache[i-3];
        }
        max_computed = n;
    }

    return cache[n];
}
