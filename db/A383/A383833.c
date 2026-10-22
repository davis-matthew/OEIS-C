/*
 * A383833  Area of the unique primitive Pythagorean triple whose inradius is
 *          A000217(n) and such that its long leg and its hypotenuse are
 *          consecutive natural numbers.
 * https://oeis.org/A383833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2 + n + 1)*(n^2 + n + 2)/4
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 50, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast index to seq_t for all arithmetic */
    seq_t n_val = (seq_t)n;
    seq_t a_term = n_val;
    seq_t b_term = n_val + 1;
    seq_t n_sq   = n_val * n_val;
    seq_t c_term = n_sq + n_val + 1; /* n^2 + n + 1 (always odd) */
    seq_t d_term = n_sq + n_val + 2; /* n^2 + n + 2 (even) */

    /* Divide by 2 twice to implement the /4, choosing any even factor each time */
    for (int i = 0; i < 2; ++i) {
        if ((a_term & 1) == 0)
            a_term /= 2;
        else if ((b_term & 1) == 0)
            b_term /= 2;
        else /* d_term must be even */
            d_term /= 2;
    }

    /* Compute the product */
    seq_t result = a_term * b_term;
    result *= c_term;
    result *= d_term;

    return result;
}
