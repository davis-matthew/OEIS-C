/*
 * A253130  Number of length 2+2 0..n arrays with the sum of medians of
 *          adjacent triples multiplied by some arrangement of +-1 equal to
 *          zero.
 * https://oeis.org/A253130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 8*n^3 + 14*n^2 + 10*n)/3 + 1
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // Convert index to seq_t once to avoid repeated casts.
    seq_t tn = (seq_t)n;
    seq_t tn2 = tn * tn;           // n^2
    seq_t tn3 = tn2 * tn;          // n^3
    seq_t tn4 = tn2 * tn2;         // n^4

    // numerator = n^4 + 8*n^3 + 14*n^2 + 10*n
    seq_t numerator = tn4
                    + (seq_t)8 * tn3
                    + (seq_t)14 * tn2
                    + (seq_t)10 * tn;

    // Since the numerator is always divisible by 3, the division is exact.
    seq_t result = numerator / (seq_t)3 + (seq_t)1;
    return result;
}
