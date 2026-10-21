/*
 * A250626  Number of (n+1)X(2+1) 0..2 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 22, u64 n <= 66, u128 n <= 156,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A250626 – Number of (n+1)×3 0..2 arrays with given monotonicity constraints
   Recurrence (OEIS formula):
     a(n) = 6*a(n-1) -10*a(n-2) -8*a(n-3) +40*a(n-4) -22*a(n-5) -47*a(n-6)
            +56*a(n-7) +15*a(n-8) -46*a(n-9) +8*a(n-10) +16*a(n-11)
            -6*a(n-12) -2*a(n-13) +a(n-14)
   Initial values (n = 1 … 14):
     154, 654, 2347, 7342, 20743, 53847, 130848, 300960,
     662000, 1402495, 2879592, 5756842, 11251304, 21566446
   The implementation evaluates the recurrence using a circular buffer of
   the last 14 terms, so it uses O(1) memory and runs in O(n) time for each
   query.  No dynamic allocation is performed, making it valid for both C
   and C++ builds, including those where seq_t is an arbitrary‑precision
   integer class. */
#include <stddef.h>

static const seq_t _a_init[14] = {
    154ULL, 654ULL, 2347ULL, 7342ULL, 20743ULL,
    53847ULL, 130848ULL, 300960ULL, 662000ULL, 1402495ULL,
    2879592ULL, 5756842ULL, 11251304ULL, 21566446ULL
};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                 /* undefined for n<1 */

    /* Return initial value if within the known range */
    if (n <= 14) return _a_init[n - 1];

    /* Circular buffer holding the last 14 terms */
    seq_t buf[14];
    for (size_t i = 0; i < 14; ++i) buf[i] = _a_init[i];

    /* Current position in the buffer (buf[0] holds a(1) at this point) */
    size_t pos = 13;                      /* index of the latest value */

    /* Iterate from n = 15 up to the requested n */
    for (idx_t i = 15; i <= n; ++i)
    {
        /* Retrieve the 14 previous terms */
        seq_t a1  = buf[pos];
        seq_t a2  = buf[(pos + 13) % 14];
        seq_t a3  = buf[(pos + 12) % 14];
        seq_t a4  = buf[(pos + 11) % 14];
        seq_t a5  = buf[(pos + 10) % 14];
        seq_t a6  = buf[(pos + 9) % 14];
        seq_t a7  = buf[(pos + 8) % 14];
        seq_t a8  = buf[(pos + 7) % 14];
        seq_t a9  = buf[(pos + 6) % 14];
        seq_t a10 = buf[(pos + 5) % 14];
        seq_t a11 = buf[(pos + 4) % 14];
        seq_t a12 = buf[(pos + 3) % 14];
        seq_t a13 = buf[(pos + 2) % 14];
        seq_t a14 = buf[(pos + 1) % 14];

        /* Compute the recurrence using a positive/negative split */
        seq_t pos_term = a1 * 6
                       + a4 * 40
                       + a7 * 56
                       + a8 * 15
                       + a10 * 8
                       + a11 * 16
                       + a14;
        seq_t neg_term = a2 * 10
                       + a3 * 8
                       + a5 * 22
                       + a6 * 47
                       + a9 * 46
                       + a12 * 6
                       + a13 * 2;

        seq_t cur = pos_term - neg_term;     /* subtraction safe: pos≥neg */

        /* Advance the circular buffer */
        pos = (pos + 1) % 14;
        buf[pos] = cur;
    }

    /* buf[pos] now holds a(n) */
    return buf[pos];
}
