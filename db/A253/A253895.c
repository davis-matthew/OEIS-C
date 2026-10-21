/*
 * A253895  Total number of octagons in two variants of an octagon expansion
 *          after n iterations: either "side-to-side" or "vertex-to-vertex",
 *          respectively.
 * https://oeis.org/A253895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-4 - i*(-i)^n + i*i^n - 18*n + 8*n^2)/4 for n > 8, which
 *            simplifies to a(n) = (8*n^2 - 18*n - 4 + s(n)) / 4, where s(n) =
 *            0 for n mod 4 = 0 or 2, s(n) = -2 for n mod 4 = 1, s(n) = +2 for
 *            n mod 4 = 3. The first eight terms are hard‑coded because the
 *            formula only holds from n = 9 onward
 * Range:     u8 n <= 8, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* First eight terms (offset 1). */
    if (n <= 8) {
        static const seq_t small[] = {1, 3, 7, 14, 25, 41, 63, 90};
        return small[n - 1];
    }

    /* Use the closed form for n > 8. */
    seq_t nn = (seq_t)n;

    /* Compute 8*n^2 - 18*n - 4. */
    seq_t val = (seq_t)8 * nn * nn;       /* 8*n^2  */
    val -= (seq_t)18 * nn;                /* -18*n  */
    val -= (seq_t)4;                      /* -4     */

    /* Adjust by s(n). */
    uint8_t mod = n & 3;                  /* n mod 4 */
    if (mod == 1) {                       /* s(n) = -2 */
        val -= (seq_t)2;
    } else if (mod == 3) {                /* s(n) = +2 */
        val += (seq_t)2;
    }

    /* Final division by 4. */
    return val / 4;
}
