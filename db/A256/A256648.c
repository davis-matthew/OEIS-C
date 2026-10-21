/*
 * A256648  28-gonal pyramidal numbers: a(n) = n*(n+1)*(26*n-23)/6.
 * https://oeis.org/A256648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(26*n-23)/6
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 997, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0; for n >= 1, a(n) = a(n-1) + 13*n*n - 12*n */
seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        idx_t temp = 13 * i;          /* 13*i fits into idx_t (uint64_t) */
        seq_t term = ii * ((seq_t)temp - 12);
        result += term;
    }
    return result;
}
