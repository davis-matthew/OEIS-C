/*
 * A321048  Number of permutations of [n] with no fixed points where the
 *          maximal displacement of an element equals two.
 * https://oeis.org/A321048
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-5) + a(n-6) Initial values: a(2)=0,
 *            a(3)=2, a(4)=3, a(5)=6, a(6)=12, a(7)=24 (recurrence holds for n
 *            ≥ 8)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 37, u64 n <= 72, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* For indices below the offset return 0. */
    if (n < (idx_t)2)
        return (seq_t)0;

    /* Static cache for memoisation. */
    static seq_t *buf     = NULL;
    static size_t capacity = 0;   /* allocated size */
    static idx_t  computed = 1;   /* largest index computed so far */

    /* Ensure the buffer can hold indices up to n and
       at least the base cases (up to 7). */
    size_t need = (size_t)n + 1;
    if (need < 8) need = 8;                 /* space for a(2)..a(7) */

    if (capacity < need) {
        size_t newcap = capacity ? capacity * 2 : 16;
        if (newcap < need)
            newcap = need;
        seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);      /* allocation failure */
        buf = tmp;
        capacity = newcap;
    }

    /* Initialise base cases only once. */
    if (computed < (idx_t)7) {
        buf[2] = (seq_t)0;
        buf[3] = (seq_t)2;
        buf[4] = (seq_t)3;
        buf[5] = (seq_t)6;
        buf[6] = (seq_t)12;
        buf[7] = (seq_t)24;
        computed = (idx_t)7;
    }

    /* If the requested value is already computed, return it. */
    if (n <= computed)
        return buf[(size_t)n];

    /* Compute consecutive terms up to n using the recurrence. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val = 2 * buf[(size_t)(i - 1)]
                    - 2 * buf[(size_t)(i - 5)]
                    + buf[(size_t)(i - 6)];
        buf[(size_t)i] = val;
    }
    computed = n;
    return buf[(size_t)n];
}
