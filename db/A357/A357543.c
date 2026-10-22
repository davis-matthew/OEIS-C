/*
 * A357543  a(n) = (3*n+1)!/(3^n*n!) * Product_{k=1..n} (3*k - 2), for n >= 0.
 * https://oeis.org/A357543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n+1)!/(3^n*n!) * ∏_{k=1..n}(3*k-2) Recurrence: a(0)=1;
 *            for n≥1, a(n)=a(n-1)*(3*n-1)*(3*n+1)*(3*n-2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t term = (seq_t)(3*i-1) * (seq_t)(3*i+1) * (seq_t)(3*i-2);
        result *= term;
    }
    return result;
}
