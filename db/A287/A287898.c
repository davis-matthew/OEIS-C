/*
 * A287898  Number of ways to go up and down n stairs, with fewer than 4
 *          stairs at a time, stepping on each stair at least once.
 * https://oeis.org/A287898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 2*a(n-3) + a(n-4), n ≥ 5 Initial
 *            values: a(1)=1, a(2)=3, a(3)=9, a(4)=27
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *buf = NULL;   /* buf[0] = a(1), buf[1] = a(2), ... */
    static size_t cap = 0;      /* number of allocated elements */
    static idx_t max_n = 0;     /* largest n for which buf[n-1] is valid */

    if (n == 0) return 0;                /* outside defined offset */

    /* Ensure capacity for indices up to n */
    if (cap < (size_t)n) {
        size_t newcap = cap ? cap * 2 : (size_t)n;
        if (newcap < (size_t)n) newcap = (size_t)n;
        buf = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* Initialise the first four terms once */
    if (max_n < 4) {
        /* initial values */
        const seq_t init[4] = { (seq_t)1, (seq_t)3, (seq_t)9, (seq_t)27 };
        size_t upto = n < 4 ? n : 4;          /* only initialise needed positions */
        for (size_t i = 0; i < upto; ++i) {
            buf[i] = init[i];
        }
        if (max_n < (idx_t)upto) max_n = (idx_t)upto;
    }

    /* Compute further terms if necessary */
    if (max_n < n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* i ≥ 5 because i > max_n ≥ 4 at this point */
            seq_t a1 = buf[i - 2];   /* a(i-1) */
            seq_t a2 = buf[i - 3];   /* a(i-2) */
            seq_t a3 = buf[i - 4];   /* a(i-3) */
            seq_t a4 = buf[i - 5];   /* a(i-4) */
            buf[i - 1] = 2 * a1 + 2 * a2 + 2 * a3 + a4;
        }
        max_n = n;
    }

    return buf[n - 1];
}
