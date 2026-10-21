/*
 * A244620  Initial terms of Erdős-Wood intervals of length 22.
 * https://oeis.org/A244620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9699690*n - 4849867 - 2192597*(-1)^n)/2
 * Range:     u8 none, u16 none, u32 n <= 489, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 9699690*n */
    seq_t t = (seq_t)9699690 * (seq_t)n;
    /* Subtract the constant part 4849867 */
    t -= (seq_t)4849867;
    /* Adjust according to parity of n:
       if n is even, subtract 2192597;
       if n is odd,  add 2192597. */
    if ((n & 1) == 0)
        t -= (seq_t)2192597;
    else
        t += (seq_t)2192597;
    /* Divide by 2 (the numerator is always even) */
    return t / (seq_t)2;
}
