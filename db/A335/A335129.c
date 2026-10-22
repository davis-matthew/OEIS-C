/*
 * A335129  a(n) is the number of distinct lines created inside an n-gon when
 *          connecting vertex k to vertex 2k mod n.
 * https://oeis.org/A335129
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n = 3; a(n) = n - 4 + (n mod 3 != 0) for n >= 4
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3)
        return 0;
    seq_t res = (seq_t)n - 4;
    if (n % 3 != 0)
        res += 1;
    return res;
}
