/*
 * A264106  Number of (n+1) X (2+1) arrays of permutations of 0..n*3+2 with
 *          each element having index change +-(.,.) 0,0 0,2 or 1,1.
 * https://oeis.org/A264106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A264106: a(n) = 4*a(n-1) + 4*a(n-3) + a(n-4) for n>4
   Initial values:
   a(1) = 9,  a(2) = 40,  a(3) = 169,  a(4) = 714   */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly */
    if (n == 1u) return (seq_t)9;
    if (n == 2u) return (seq_t)40;
    if (n == 3u) return (seq_t)169;
    if (n == 4u) return (seq_t)714;

    /* Keep the last four computed values:
       t[0] = a(i-4), t[1] = a(i-3), t[2] = a(i-2), t[3] = a(i-1) */
    seq_t t[4];
    t[0] = (seq_t)9;    /* a(1) */
    t[1] = (seq_t)40;   /* a(2) */
    t[2] = (seq_t)169;  /* a(3) */
    t[3] = (seq_t)714;  /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = 4 * t[3] + 4 * t[1] + t[0]; /* a(i) = 4*a(i-1)+4*a(i-3)+a(i-4) */
        t[0] = t[1];
        t[1] = t[2];
        t[2] = t[3];
        t[3] = next;
    }

    return t[3];
}
