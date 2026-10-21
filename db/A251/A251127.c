/*
 * A251127  Number of (n+1) X (7+1) 0..1 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements greater than the maximum of its
 *          antidiagonal elements.
 * https://oeis.org/A251127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 15, u32 n <= 38, u64 n <= 84, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A251127
   Recurrence (for n > 6):
     a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5)
     a(1) = 354
     a(2) = 510
     a(3) = 680
     a(4) = 938
     a(5) = 1268
     a(6) = 1743
   This implementation computes a(n) directly using the recurrence
   with a sliding window of the last five terms.  It is valid for any
   n ≥ 1, independent of previous calls.  The routine runs in O(n)
   time and uses only a few words of memory. */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values 1‑based; index 0 unused */
    static const seq_t base[7] = {
        0,          /* a(0) not defined */
        354,        /* a(1) */
        510,        /* a(2) */
        680,        /* a(3) */
        938,        /* a(4) */
        1268,       /* a(5) */
        1743        /* a(6) */
    };

    if (n <= 6) {
        return base[(size_t)n];
    }

    /* Sliding window for the last five terms:
         t1 = a(i-1), t2 = a(i-2), t3 = a(i-3), t4 = a(i-4), t5 = a(i-5)  */
    seq_t t1 = base[6];  /* a(6) */
    seq_t t2 = base[5];  /* a(5) */
    seq_t t3 = base[4];  /* a(4) */
    seq_t t4 = base[3];  /* a(3) */
    seq_t t5 = base[2];  /* a(2) */
    seq_t next = 0;

    for (idx_t i = 7; i <= n; ++i) {
        next = 4*t1 - 5*t2 + t3 + 2*t4 - t5;

        /* Shift the window for next iteration */
        t5 = t4;
        t4 = t3;
        t3 = t2;
        t2 = t1;
        t1 = next;
    }

    return next;
}
