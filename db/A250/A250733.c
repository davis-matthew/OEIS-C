/*
 * A250733  Number of (4+1) X (n+1) 0..1 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) - 13*a(n-3) + 15*a(n-4) + 3*a(n-5) -
 *            11*a(n-6) + 4*a(n-7) for n > 9 Initial values: a(1) = 58, a(2) =
 *            144, a(3) = 365, a(4) = 885, a(5) = 2092, a(6) = 4889, a(7) =
 *            11377, a(8) = 26419, a(9) = 61330
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 22, u64 n <= 48, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* for size_t */

/* Implementation of the recurrence, computed iteratively.  No caching is
   required – recomputing from the known seed values suffices for n ≤ 10 000. */
seq_t a(idx_t n)
{
    /* Base terms (indices 1..9) */
    static const seq_t base[10] = {
        0,      /* unused 0‑th entry */
        58,     /* a(1) */
        144,    /* a(2) */
        365,    /* a(3) */
        885,    /* a(4) */
        2092,   /* a(5) */
        4889,   /* a(6) */
        11377,  /* a(7) */
        26419,  /* a(8) */
        61330   /* a(9) */
    };

    if (n < 1)
        return 0;          /* undefined for n < 1 */

    if (n <= 9)
        return base[(size_t)n];

    /* Window holds a(n-7) … a(n-1) for the current step.  For k = 10,
       the window is a(3) … a(9). */
    seq_t window[7];
    window[0] = base[3];
    window[1] = base[4];
    window[2] = base[5];
    window[3] = base[6];
    window[4] = base[7];
    window[5] = base[8];
    window[6] = base[9];

    for (idx_t k = 10; k <= n; ++k) {
        seq_t next = window[6] * 4
                    - window[5]
                    - window[4] * 13
                    + window[3] * 15
                    + window[2] * 3
                    - window[1] * 11
                    + window[0] * 4;

        /* Shift window left by one position (discard a(n-7)). */
        window[0] = window[1];
        window[1] = window[2];
        window[2] = window[3];
        window[3] = window[4];
        window[4] = window[5];
        window[5] = window[6];
        window[6] = next;
    }

    return window[6];      /* a(n) */
}
