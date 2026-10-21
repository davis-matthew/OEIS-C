/*
 * A273454  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 734", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*4^n - 33*2^n + 757 for n >= 7; a(0) = 1, a(1) = 5, a(2)
 *            = 21, a(3) = 81, a(4) = 333, a(5) = 1493, a(6) = 6781
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pre[7] = {
        (seq_t)1,     /* a(0) */
        (seq_t)5,     /* a(1) */
        (seq_t)21,    /* a(2) */
        (seq_t)81,    /* a(3) */
        (seq_t)333,   /* a(4) */
        (seq_t)1493,  /* a(5) */
        (seq_t)6781   /* a(6) */
    };

    if (n <= 6)
        return pre[n];

    /* For n >= 7 compute 2*4^n - 33*2^n + 757 */
    seq_t pow2 = (seq_t)1;
    pow2 <<= n;            /* pow2 = 2^n */
    seq_t pow4 = pow2;
    pow4 <<= n;            /* pow4 = 2^(2n) = 4^n */

    seq_t result = pow4;
    result <<= 1;          /* result = 2*4^n */

    result -= (pow2 * (seq_t)33); /* subtract 33*2^n */
    result += (seq_t)757;        /* add constant 757 */

    return result;
}
