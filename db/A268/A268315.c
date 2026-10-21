/*
 * A268315  Decimal expansion of 256/27.
 * https://oeis.org/A268315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(1)=9; for n>1, a(n)=4,8,1 repeating with period 3
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1U) {
        return (seq_t)9U;
    }
    idx_t r = (n - 2U) % 3U;   /* 0->4, 1->8, 2->1 */
    if (r == 0U) {
        return (seq_t)4U;
    } else if (r == 1U) {
        return (seq_t)8U;
    } else {
        return (seq_t)1U;
    }
}
