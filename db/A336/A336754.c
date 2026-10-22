/*
 * A336754  Perimeters in increasing order of integer-sided triangles whose
 *          sides a < b < c are in arithmetic progression.
 * https://oeis.org/A336754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * A307136(n) A307136(n) is the n-th integer b ≥ 3 such
 *            that the cumulative number of triangles with middle side ≤ b
 *            reaches n: cum(b) = Σ_{k=3}^{b} ⌊(k‑1)/2⌋. The perimeter of the
 *            n‑th triangle (sides a<b<c in AP) equals 3b
 * Range:     u8 n <= 1764, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */
    idx_t target = n;
    idx_t cum    = 0;
    idx_t b      = 3;               /* smallest possible middle side */

    for (;; ++b) {
        idx_t count = (b - 1) / 2;   /* number of triangles with this b */
        if (cum + count >= target) {
            return (seq_t)3 * b;    /* perimeter = 3 * middle side */
        }
        cum += count;
    }
}
