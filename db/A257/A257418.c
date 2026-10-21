/*
 * A257418  Number of pieces after a sheet of paper is folded n times and cut
 *          diagonally.
 * https://oeis.org/A257418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A257418 (Number of pieces after a sheet of paper is folded n times
   and cut diagonally).
   Formula used:
       a(0) = 2, a(1) = 3, a(2) = 5,
       for n > 2  :  a(n) = 2*a(n-1) - 2^floor((n-2)/2) - 1
   The recurrence follows from a(n+1) = 2*a(n) - 2^floor((n-1)/2) - 1
   for n > 1, applied with n = n-1.  This implementation
   evaluates the recurrence directly in O(n) time. */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;

    /* For n >= 3 compute iteratively. */
    seq_t a_prev = (seq_t)5;          /* a(2) */
    seq_t pow2   = (seq_t)1;          /* 2^floor((3-2)/2) = 2^0 */

    for (idx_t k = 3; k <= n; ++k) {
        seq_t temp = a_prev + a_prev; /* 2 * a_prev */
        temp -= pow2;                 /* subtract 2^floor((k-2)/2) */
        temp -= (seq_t)1;             /* subtract 1         */
        a_prev = temp;                /* a(k) is now stored */

        /* Update pow2 for the next iteration.
           floor((k-2)/2) increments by 1 exactly when k is odd. */
        if ((k & 1u) != 0u)
            pow2 += pow2;             /* pow2 *= 2 */
    }

    return a_prev;
}
