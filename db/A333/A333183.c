/*
 * A333183  Number of digits in concatenation of first n positive even
 *          integers.
 * https://oeis.org/A333183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{i=1..n} (1 + floor(log_10(2*i)))
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Number of digits in the concatenation of the first n positive even integers. */

seq_t a(idx_t n)
{
    seq_t total = 0;                 /* accumulator for the answer */
    for (idx_t i = 1; i <= n; ++i) {
        /* value of the i-th even integer : 2*i */
        seq_t val = ((seq_t)2 * (seq_t)i);

        /* count decimal digits of val */
        unsigned int digits = 0;
        while (val > 0) {
            val /= 10;
            ++digits;
        }

        /* add the digit count to the total */
        total += (seq_t)digits;
    }
    return total;
}
