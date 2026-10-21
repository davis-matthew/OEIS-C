/*
 * A249101  p(n,1), where p(n,x) is defined in Comments; sum of numbers in row
 *          n of the array at A249100.
 * https://oeis.org/A249101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + (2*n-1)*a(n-2), a(0) = a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) return 1;
    seq_t a0 = 1;  /* a(n-2) */
    seq_t a1 = 1;  /* a(n-1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t a_new = a1 + ((seq_t)(2 * i - 1) * a0);
        a0 = a1;
        a1 = a_new;
    }
    return a1;
}
