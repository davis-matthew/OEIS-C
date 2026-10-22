/*
 * A302914  Determinant of n X n matrix whose main diagonal consists of the
 *          first n 10-gonal numbers and all other elements are 1's.
 * https://oeis.org/A302914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n+1)=a(n)*n*(4*n+5)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, but safe fallback */
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        result *= (seq_t)i * ((seq_t)(4 * i) + 5);
    }
    return result;
}
