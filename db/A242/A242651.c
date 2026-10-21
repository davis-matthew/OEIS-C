/*
 * A242651  Real part of Product_{k=0..n} (i-k), where i = sqrt(-1).
 * https://oeis.org/A242651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Re[∏_{k=0}^n (i - k)]
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the real part of the product (i)(i-1)(i-2)...(i-n)   */
/* using the recurrence:                                           */
/*   R_{k} = -(k * R_{k-1}) - I_{k-1}                              */
/*   I_{k} =  R_{k-1} - (k * I_{k-1})                              */
/* where (R_0, I_0) = (0,1).                                       */

seq_t a(idx_t n)
{
    seq_t R = 0;   /* real part  */
    seq_t I = 1;   /* imaginary part */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* compute intermediate products   */
        seq_t prodR = (seq_t)k * R;
        seq_t prodI = (seq_t)k * I;

        /* update real and imaginary parts   */
        seq_t newR = -prodR - I;
        seq_t newI = R - prodI;

        R = newR;
        I = newI;
    }

    return R;  /* a(n) is the real part   */
}
