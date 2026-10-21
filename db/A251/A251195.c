/*
 * A251195  Number of (n+1) X (2+1) 0..1 arrays with no 2 X 2 subblock having
 *          the minimum of its diagonal elements less than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A251195
 *  Recurrence: a(n) = 2*a(n-1) + 7*a(n-2) - a(n-3) - 7*a(n-4) - a(n-5)   (n > 6)
 *  Initial values: a(1)=25, a(2)=86, a(3)=310, a(4)=1135, a(5)=4172, a(6)=15353
 *  This implementation evaluates the recurrence iteratively, using only
 *  integer arithmetic on type seq_t.  No additional resources are allocated.
 */
#include <stddef.h>
#include <stdint.h>

static const seq_t base[6] = {
    (seq_t)25,    /* a(1) */
    (seq_t)86,    /* a(2) */
    (seq_t)310,   /* a(3) */
    (seq_t)1135,  /* a(4) */
    (seq_t)4172,  /* a(5) */
    (seq_t)15353  /* a(6) */
};

seq_t a(idx_t n)
{
    if (n < (idx_t)1) {
        /* Undefined for n < 1 – return 0 as a safe default */
        return (seq_t)0;
    }
    if (n <= (idx_t)6) {
        return base[(size_t)(n - 1)];
    }

    /* Window of five preceding terms: a(n-5) … a(n-1) pre‑initialised for n=7 */
    seq_t prev[5];
    prev[0] = base[1]; /* a(2) */
    prev[1] = base[2]; /* a(3) */
    prev[2] = base[3]; /* a(4) */
    prev[3] = base[4]; /* a(5) */
    prev[4] = base[5]; /* a(6) */

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute a(i) using the recurrence relation */
        seq_t next = (seq_t)2 * prev[4]
                   + (seq_t)7 * prev[3]
                   - prev[2]
                   - (seq_t)7 * prev[1]
                   - prev[0];
        /* Shift the window one step forward */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = next;
    }

    return prev[4]; /* a(n) */
}
