/*
 * A384917  Decimal expansion of 1/3645.
 * https://oeis.org/A384917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the n-th digit after the decimal point in the expansion
 *            of 1/3645. The digits are obtained by successive long‑division:
 *            at each step r := r * 10; digit := r / 3645; r := r % 3645; with
 *            initial r = 1
 * Range:     u8 n <= 2, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t r = 1;
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;
        digit = r / 3645;
        r %= 3645;
    }
    return digit;
}
