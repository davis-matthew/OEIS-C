/*
 * A249056  a(n) = Product_{i=1..floor(n/2)} (2n-i)*(2n+i).
 * https://oeis.org/A249056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..floor(n/2)} (2n-i)*(2n+i)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;
    seq_t result = (seq_t)1;
    seq_t two_n   = (seq_t)n * ((seq_t)2);
    idx_t limit   = n / 2;
    for (idx_t i = 1; i <= limit; ++i) {
        seq_t term1 = two_n - (seq_t)i;
        seq_t term2 = two_n + (seq_t)i;
        result *= term1;
        result *= term2;
    }
    return result;
}
