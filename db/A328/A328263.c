/*
 * A328263  a(n) = number of letters in a(n-1) (in Polish), with a(1) = 1.
 * https://oeis.org/A328263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   a(n) = a(n-3) for n>4; a(1)=1, a(2)=5, a(3)=4, a(4)=6
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    static const seq_t pattern[3] = {5, 4, 6};
    return pattern[(n - 2) % 3];
}
