/*
 * A305750  Number of achiral color patterns (set partitions) in a row or
 *          cycle of length n with 4 or fewer colors (subsets).
 * https://oeis.org/A305750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=3; a(n)=a(n-1)+4*a(n-2)-4*a(n-3)
 *            for n>=4 derived from the generating function
 *            (1-3x^2+x^3)/((1-x)(1-4x^2))
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef SEQ_A305750_H
#define SEQ_A305750_H

seq_t a(idx_t n)
{
    /* Handle the four base cases directly */
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 3;

    /* Initialise the three most recent terms:
        a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a1 = 1;   /* a(1) */
    seq_t a2 = 2;   /* a(2) */
    seq_t a3 = 3;   /* a(3) */
    idx_t i;

    /* Compute successive terms up to a(n) */
    for (i = 4; i <= n; ++i) {
        /* recurrence: a(i) = a3 + 4*a2 - 4*a1 */
        seq_t next = a3 + ((a2 << 2)) - ((a1 << 2));
        /* shift the window one step forward */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}

#endif /* SEQ_A305750_H */
