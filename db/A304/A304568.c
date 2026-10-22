/*
 * A304568  Number of minimum dominating sets in the n-antiprism graph.
 * https://oeis.org/A304568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Piecewise formulas depending on n mod 5 (proved by Andrew
 *            Howroyd)
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 567, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k   = n / 5;
    seq_t sk  = (seq_t)k;
    switch (n % 5) {
        case 0:
            return (seq_t)5;
        case 1: {
            seq_t v = (seq_t)5 * sk + (seq_t)1;
            v *= (sk + (seq_t)1);
            v *= ((seq_t)2 * sk + (seq_t)3);
            v /= (seq_t)3;
            v *= (seq_t)2;
            return v;
        }
        case 2:
            return (seq_t)2 * ((seq_t)n);
        case 3: {
            seq_t v = sk * (seq_t)5 + (seq_t)3;           /* n = 5k+3 */
            v *= ((seq_t)2 * sk + (seq_t)5);
            v *= ((seq_t)2 * sk + (seq_t)4);
            v *= ((seq_t)2 * sk + (seq_t)3);
            v /= (seq_t)12;
            return v;
        }
        case 4:
            return (sk * (seq_t)5 + (seq_t)4) * ((seq_t)2 * sk + (seq_t)3);
    }
    return 0; /* unreachable */
}
