/*
 * A395828  Number of septenary strings of length n that contain a specific
 *          string xx (where x is a single digit).
 * https://oeis.org/A395828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - 36*a(n-2) - 42*a(n-3), with a(0)=0, a(1)=0,
 *            a(2)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* External types supplied by the build. */
extern seq_t a(idx_t n);

/* Compute a(n) by an explicit recurrence (O(n)). */
seq_t a(idx_t n)
{
    /* Handle the three known initial values. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* Three sliding‑window variables holding a(n-3), a(n-2), a(n-1). */
    seq_t a_nm3 = (seq_t)0;      /* a(0)   */
    seq_t a_nm2 = (seq_t)0;      /* a(1)   */
    seq_t a_nm1 = (seq_t)1;      /* a(2)   */
    seq_t a_n   = (seq_t)0;      /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        /* a(n) = 13*a(n-1) - 36*a(n-2) - 42*a(n-3) */
        seq_t t1 = a_nm1 * (seq_t)13;
        seq_t t2 = a_nm2 * (seq_t)36;
        seq_t t3 = a_nm3 * (seq_t)42;
        a_n = t1 - t2 - t3;

        /* Shift the window for the next iteration. */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
