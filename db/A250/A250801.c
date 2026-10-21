/*
 * A250801  Number of (4+1) X (n+1) 0..1 arrays with nondecreasing
 *          min(x(i,j),x(i,j-1)) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 9*a(n-3) + 9*a(n-4) + 6*a(n-5) -
 *            8*a(n-6) - a(n-7) + 2*a(n-8) Base values: a(1)=140, a(2)=490,
 *            a(3)=1430, a(4)=3776, a(5)=9258, a(6)=21610, a(7)=48600,
 *            a(8)=106426
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 22, u64 n <= 54, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined for n < 1 */

    if (n == 1) return (seq_t)140;
    if (n == 2) return (seq_t)490;
    if (n == 3) return (seq_t)1430;
    if (n == 4) return (seq_t)3776;
    if (n == 5) return (seq_t)9258;
    if (n == 6) return (seq_t)21610;
    if (n == 7) return (seq_t)48600;
    if (n == 8) return (seq_t)106426;

    /* Keep the last eight terms to apply the recurrence. */
    seq_t t1 = (seq_t)140;   /* a(n-8) */
    seq_t t2 = (seq_t)490;   /* a(n-7) */
    seq_t t3 = (seq_t)1430;  /* a(n-6) */
    seq_t t4 = (seq_t)3776;  /* a(n-5) */
    seq_t t5 = (seq_t)9258;  /* a(n-4) */
    seq_t t6 = (seq_t)21610; /* a(n-3) */
    seq_t t7 = (seq_t)48600; /* a(n-2) */
    seq_t t8 = (seq_t)106426;/* a(n-1) */

    seq_t next;
    for (idx_t i = 9; i <= n; ++i) {
        /* Compute a(i) using the recurrence. */
        next = 4 * t8
             - 2 * t7
             - 9 * t6
             + 9 * t5
             + 6 * t4
             - 8 * t3
             - t2
             + 2 * t1;

        /* Shift the window of last eight terms. */
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = t5;
        t5 = t6;
        t6 = t7;
        t7 = t8;
        t8 = next;
    }

    return t8; /* t8 now holds a(n) */
}
