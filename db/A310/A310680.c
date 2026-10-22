/*
 * A310680  Coordination sequence Gal.5.4.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n + 1 + 2*floor(n/8) + extra(n) Define b = floor(n/8) r
 *            = n mod 8 Then extra(n) = { 3*(b-1) + 2 if b ≥ 1 and r = 0, 3*b
 *            if b ≥ 1 and r = 1, 0 otherwise } The function works for any n ≥
 *            0 and uses only integer arithmetic
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t b = n / 8;          /* floor(n/8)   */
    idx_t r = n % 8;          /* n mod 8      */
    seq_t extra = 0;

    if (b >= 1)
    {
        if (r == 0)
            extra = (seq_t)(3 * (seq_t)(b - 1) + 2);
        else if (r == 1)
            extra = (seq_t)(3 * (seq_t)b);
    }

    /* a(n) = 3*n + 1 + 2*b + extra */
    seq_t result = (seq_t)n * 3 + 1 + (seq_t)(2 * b) + extra;
    return result;
}
