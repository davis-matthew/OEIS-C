/*
 * A266218  Decimal representation of the n-th iteration of the "Rule 7"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n==0; 6 if n==1; 0 if n even >0; a(n) = 2^(2*n+1) -
 *            1 for odd n >= 3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 6;
    if ((n & 1) == 0) return 0;

    idx_t exp = 2 * n + 1;
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) {
        res <<= 1;
    }
    res -= 1;
    return res;
}
