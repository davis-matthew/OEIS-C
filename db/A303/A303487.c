/*
 * A303487  a(n) = n! * [x^n] 1/(1 - 4*x)^(n/4).
 * https://oeis.org/A303487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0}^{n-1} (4k + n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to a(n) = n! * [x^n] 1/(1-4x)^(n/4) */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    for (idx_t k = 0; k < n; ++k) {
        result *= (seq_t)(4 * k + n);
    }
    return result;
}
