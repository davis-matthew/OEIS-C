/*
 * A310526  Coordination sequence Gal.6.332.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, let f(n) = #{ k∈[2,n] : k mod 11 ∈{0,1,6} }
 *            (count of numbers that are multiples of 11, 1‑mod‑11 or
 *            6‑mod‑11, excluding the special value 1). Then a(n) = 6*n - 2 -
 *            2*f(n). The count f(n) can be written with integer divisions as:
 *            f(n) = ⌊n/11⌋ + ⌊(n-1)/11⌋ + (n>=6 ? ⌊(n-6)/11⌋ + 1...
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Count numbers 0 mod 11, 1 mod 11 (excluding 1), and 6 mod 11 */
    idx_t cnt0 = n / 11;                    /* multiples of 11 */
    idx_t cnt1 = (n >= 1) ? (n - 1) / 11 : 0; /* numbers ≡1 (mod 11) except 1 */
    idx_t cnt6 = 0;
    if (n >= 6)
        cnt6 = (n - 6) / 11 + 1;           /* numbers ≡6 (mod 11) */

    idx_t f = cnt0 + cnt1 + cnt6;

    /* a(n) = 6*n - 2 - 2*f, computed in seq_t to avoid overflow */
    seq_t nn = (seq_t)n;
    seq_t ff = (seq_t)f;
    return (seq_t)6 * nn - (seq_t)2 - (seq_t)2 * ff;
}
