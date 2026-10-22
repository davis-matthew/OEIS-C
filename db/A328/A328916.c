/*
 * A328916  Number of n-digit decimal numbers containing 123 as a substring.
 * https://oeis.org/A328916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*a(n-1) + 9*10^(n-4) - a(n-3) Initial values: a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small indices directly */
    if (n < 3)           return (seq_t)0;
    if (n == 3)          return (seq_t)1;

    /* a(i-3), a(i-2) and a(i-1) for the current i */
    seq_t a_im3 = (seq_t)0;   /* a(1) */
    seq_t a_im2 = (seq_t)0;   /* a(2) */
    seq_t a_im1 = (seq_t)1;   /* a(3) */

    /* 10^(i-4) for the current i, starting with i = 4 → 10^0 = 1 */
    seq_t pow10 = (seq_t)1;   /* 10^(i-4) */

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute a(i) = 10*a(i-1) + 9*10^(i-4) - a(i-3) */
        seq_t term = a_im1 * (seq_t)10 + (seq_t)9 * pow10 - a_im3;

        /* Shift the window for the next iteration */
        a_im3 = a_im2;
        a_im2 = a_im1;
        a_im1 = term;

        /* Update the power of 10 for the next i */
        pow10 *= (seq_t)10;
    }

    return a_im1;   /* a(n) */
}
