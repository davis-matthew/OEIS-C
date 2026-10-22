/*
 * A394371  a(n) = (16*n)!/((2*n)!*(4*n)!*(10*n)!).
 * https://oeis.org/A394371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (16*n)!/((2*n)!*(4*n)!*(10*n)!)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute binomial(16n,6n) * binomial(6n,2n) using multiplicative formula */
    seq_t res = 1;
    idx_t six_n  = 6 * n;
    idx_t ten_n  = 10 * n;

    /* binomial(16n,6n) = product_{i=1}^{6n} (10n + i) / i */
    for (idx_t i = 1; i <= six_n; ++i) {
        res = (res * ((seq_t)ten_n + (seq_t)i)) / (seq_t)i;
    }

    idx_t two_n = 2 * n;
    idx_t four_n = 4 * n;

    /* binomial(6n,2n) = product_{i=1}^{2n} (4n + i) / i */
    for (idx_t i = 1; i <= two_n; ++i) {
        res = (res * ((seq_t)four_n + (seq_t)i)) / (seq_t)i;
    }

    return res;
}
