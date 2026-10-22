/*
 * A377726  Length of the long leg of the unique primitive Pythagorean triple
 *          (x,y,z) such that (x-y+z)/2 is A002315(n) and such that its long
 *          leg and its hypotenuse are consecutive natural numbers.
 * https://oeis.org/A377726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 2 * A002315(2*n) * (A002315(2*n) - 1)
 * 
 * A002315 is the sequence of numerators of the convergents of sqrt(2),
 * defined by the recurrence
 *   A002315(0) = 1,  A002315(1) = 3
 *   A002315(m) = 2*A002315(m-1) + A002315(m-2)   for m ≥ 2
 * 
 * This function first computes A002315(2*n) by iterating the recurrence
 * up to the required index, then returns the value of the closed form.
 */

seq_t a(idx_t n)
{
    /* index in the A002315 sequence */
    idx_t m = (idx_t)2 * n;          /* 2*n */

    /* base terms of the recurrence */
    seq_t k_prev2 = (seq_t)1;       /* A002315(0) */
    seq_t k_prev1 = (seq_t)3;       /* A002315(1) */
    seq_t k;                        /* A002315(m) */

    if (m == 0) {
        k = k_prev2;
    } else if (m == 1) {
        k = k_prev1;
    } else {
        seq_t k_cur;
        for (idx_t i = (idx_t)2; i <= m; ++i) {
            /* k_cur = 2*k_prev1 + k_prev2 */
            k_cur = (seq_t)2 * k_prev1 + k_prev2;
            k_prev2 = k_prev1;
            k_prev1 = k_cur;
        }
        k = k_prev1;
    }

    /* a(n) = 2 * k * (k - 1) */
    return (seq_t)2 * k * (k - (seq_t)1);
}
