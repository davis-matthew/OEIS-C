/*
 * A344919  a(n) = n^n - n*(n + 1) / 2.
 * https://oeis.org/A344919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n - n*(n+1)/2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to the sequence type once */
    seq_t sn = (seq_t)n;

    /* Fast exponentiation: compute n^n in O(log n) multiplications */
    seq_t pow_n = 1;          /* accumulator for the result   */
    seq_t base   = sn;        /* current power of n           */
    idx_t exp   = n;          /* exponent to process          */

    while (exp) {
        if (exp & 1U)
            pow_n = pow_n * base;
        base = base * base;
        exp >>= 1U;
    }

    /* Triangular number n*(n+1)/2 */
    seq_t tri = sn * (sn + (seq_t)1) / (seq_t)2;

    return pow_n - tri;
}
