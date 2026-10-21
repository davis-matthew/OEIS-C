/*
 * A269532  Number of length-n 0..3 arrays with no repeated value differing
 *          from the previous repeated value by other than one.
 * https://oeis.org/A269532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 9*a(n-2) -23*a(n-3) +31*a(n-4) +33*a(n-5)
 *            initial terms: a(1)=4, a(2)=16, a(3)=60, a(4)=222, a(5)=804
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                  /* for idx_t definition in the build */

static const seq_t base_vals[6] = {0, 4, 16, 60, 222, 804};

seq_t a(idx_t n)
{
    /* Protect against the offset being below 1.  For such calls
       simply return 0, though the problem statement specifies that
       behaviour for n < 1 does not matter. */
    if (n < 1) return 0;

    /* Return base case values directly. */
    if (n <= 5) return base_vals[n];

    /* Window of the last five computed values:
       a1 = a(n-5), a2 = a(n-4), a3 = a(n-3), a4 = a(n-2), a5 = a(n-1) */
    seq_t a1 = base_vals[1];
    seq_t a2 = base_vals[2];
    seq_t a3 = base_vals[3];
    seq_t a4 = base_vals[4];
    seq_t a5 = base_vals[5];

    seq_t next = a5;   /* placeholder, will be overwritten */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t pos = a5 * 7 + a2 * 31 + a1 * 33;  /* 7*a5 + 31*a2 + 33*a1 */
        seq_t neg = a4 * 9 + a3 * 23;            /* 9*a4 + 23*a3   */
        next = pos - neg;                       /* a(i) = pos - neg */

        /* shift the window for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }

    return next;
}
