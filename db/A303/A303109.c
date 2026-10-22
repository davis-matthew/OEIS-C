/*
 * A303109  a(n) = n*(2*n-1)*a(n-1) + ((n-1)!)^2, with a(0) = 0, n > 0.
 * https://oeis.org/A303109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(2*n-1)*a(n-1) + ((n-1)!)^2 with a(0)=0 Offset 0 – a(0)
 *            is the first term
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* a and factorial for (i-1)! at the start of the loop for i = 1 */
    seq_t a_val = (seq_t)0;
    seq_t fact  = (seq_t)1;  /* fact = (0)! = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        /* square = (i-1)!^2 */
        seq_t square = fact * fact;

        /* coefficient = i * (2*i - 1) */
        const seq_t ii = (seq_t)i;
        seq_t coeff = ii * (ii * 2 - (seq_t)1);

        /* recurrence */
        a_val = coeff * a_val + square;

        /* update factorial for next iteration: fact = i! */
        fact = fact * ii;
    }

    return a_val;
}
