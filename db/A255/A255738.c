/*
 * A255738  a(1) = 1; for n > 1, a(n) = 1*0^{A000120(n-1) - 1}.
 * https://oeis.org/A255738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(1)=1; for n>1, a(n)=1 iff (n‑1) is a power of two, else 0
 * Range:     u8 n <= 9704, u16 n <= 9704, u32 n <= 9704, u64 n <= 9704,
 *            u128 n <= 9704, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* n>1, so n-1 fits in idx_t and is non‑zero. */
    idx_t val = n - 1;
    /* val is a power of two iff val & (val-1) == 0 (for val>0). */
    if ((val & (val - 1)) == 0) {
        return 1;
    }
    return 0;
}
