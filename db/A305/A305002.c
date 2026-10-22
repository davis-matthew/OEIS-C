/*
 * A305002  Expansion of (1 + 20*x + 30*x^2 + 50*x^3 + 7*x^4 + 10*x^5)/((1 -
 *          x)*(1 + x^4)).
 * https://oeis.org/A305002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A305002
 *  a(n) = sum_{m=0}^{5} P[m] * (( (n - m) / 4 ) mod 2 == 0),
 *  where P = {1, 20, 30, 50, 7, 10}.
 *  This follows from (1+20x+30x²+50x³+7x⁴+10x⁵)/((1−x)(1+x⁴)).  */
static const idx_t P[6] = {1, 20, 30, 50, 7, 10};

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t m = 0; m <= 5 && m <= n; ++m) {
        idx_t k = n - m;
        if (((k / 4) & 1ULL) == 0)
            res += (seq_t)P[m];
    }
    return res;
}
