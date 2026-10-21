/*
 * A271800  Five steps forward, four steps back.
 * https://oeis.org/A271800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} (-1)^floor((2*i-2)/9) ; a(0)=0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t partial[9] = {0, 1, 2, 3, 4, 5, 4, 3, 2};
    return (seq_t)(n / 9) + partial[n % 9];
}
