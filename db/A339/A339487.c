/*
 * A339487  a(n) is the area of the n-gon with vertices (3^i, 5^i) for 0 <= i
 *          <= n-1.
 * https://oeis.org/A339487
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=0}^{n-2} 15^i - (5^{n-1} - 3^{n-1})/2 Derived from
 *            the shoelace formula for the polygon with vertices (3^i,5^i).
 *            For indices n < 3 the function returns 0, as the sequence is
 *            defined for n ≥ 3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 3; behaviour for smaller n
       is irrelevant. */
    if (n < 3) return 0;

    /* Accumulate powers of 15 for indices 0 .. n-2 */
    seq_t sum15 = 0;
    seq_t pow15 = 1;
    for (idx_t i = 0; i <= n - 2; ++i) {
        sum15 += pow15;          /* sum += 15^i */
        pow15 *= (seq_t)15;     /* next power */
    }

    /* Compute 5^(n-1) and 3^(n-1) */
    seq_t pow5 = 1;
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n - 1; ++i) {
        pow5 *= (seq_t)5;
        pow3 *= (seq_t)3;
    }

    /* (5^(n-1) - 3^(n-1)) / 2 is integer because the difference is even */
    seq_t diff = pow5 - pow3;
    seq_t half = diff / 2;

    /* Final area value */
    return sum15 - half;
}
