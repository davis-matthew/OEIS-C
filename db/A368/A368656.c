/*
 * A368656  Decimal expansion of 271801/99990.
 * https://oeis.org/A368656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(1)=2; a(2)=7; for n>=3, a(n) cycles through 1,8,2,8 with
 *            period 4
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 2;
    if (n == 2) return 7;
    static const seq_t cycle[4] = {1, 8, 2, 8};
    return cycle[(n - 3) & 3];
}
