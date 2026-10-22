/*
 * A303486  a(n) = n! * [x^n] 1/(1 - 3*x)^(n/3).
 * https://oeis.org/A303486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0..n-1} (3*k + n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 0; k < n; ++k) {
        result *= (seq_t)(3 * k + n);
    }
    return result;
}
