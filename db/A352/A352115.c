/*
 * A352115  Partial sums of the even triangular numbers (A014494).
 * https://oeis.org/A352115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = (n+1)*(4*n^2 + 8*n + 3 - 3*(-1)^n)/6 */
seq_t a(idx_t n)
{
    /* Cast index to seq_t for all arithmetic with the target type. */
    seq_t tn  = (seq_t)n;              /* n */
    seq_t n1  = tn + (seq_t)1;         /* n+1 */
    seq_t nsq = tn * tn;               /* n^2 */

    /* Compute the inner polynomial: 4*n^2 + 8*n + 3. */
    seq_t t = (seq_t)4 * nsq + (seq_t)8 * tn + (seq_t)3;

    /* Adjust for the (-1)^n term: add 3 if n is odd, subtract 3 if even. */
    if ((n & (idx_t)1) == 0) {
        t -= (seq_t)3;
    } else {
        t += (seq_t)3;
    }

    /* Final division by 6 (exact). */
    return n1 * t / (seq_t)6;
}
