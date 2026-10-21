/*
 * A279495  Number of tetrahedral numbers dividing n.
 * https://oeis.org/A279495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of tetrahedral divisors of n Tetrahedral numbers
 *            are k(k+1)(k+2)/6 for k ≥ 1. a(n) counts how many divisors of n
 *            equal a tetrahedral number
 * Range:     u8 n <= 511, u16 n <= 8917, u32 n <= 8917, u64 n <= 8917,
 *            u128 n <= 8917, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t n_seq = (seq_t)n;

    for (idx_t k = 1; ; k++)
    {
        seq_t a_val = (seq_t)k;
        seq_t b_val = (seq_t)(k + 1);
        seq_t c_val = (seq_t)(k + 2);

        /* Reduce the product a_val*b_val*c_val by 6 to avoid overflow
           while keeping the result equal to the tetrahedral number. */
        if (a_val % 6 == 0) {
            a_val /= 6;
        } else if (b_val % 6 == 0) {
            b_val /= 6;
        } else if (c_val % 6 == 0) {
            c_val /= 6;
        } else {
            /* divide by 2 */
            if (a_val % 2 == 0) a_val /= 2;
            else if (b_val % 2 == 0) b_val /= 2;
            else c_val /= 2;

            /* divide by 3 */
            if (a_val % 3 == 0) a_val /= 3;
            else if (b_val % 3 == 0) b_val /= 3;
            else c_val /= 3;
        }

        seq_t t_val = a_val * b_val * c_val;   /* tetrahedral number */

        if (t_val > n_seq) break;              /* further tetrahedral numbers are larger */

        if (n_seq % t_val == 0) count++;        /* divisor test */
    }

    return count;
}
