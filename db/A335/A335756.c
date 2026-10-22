/*
 * A335756  A cup filling problem starting with 2 empty cups of sizes 3 and n,
 *          where a(n) is the number of unreachable states (see details in
 *          comments).
 * https://oeis.org/A335756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*(n/3)+2 if n mod 3 == 0, otherwise a(n) = 2*(n-1)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n % 3 == 0) {
        return (seq_t)10 * (seq_t)(n / 3) + (seq_t)2;
    } else {
        return (seq_t)2 * (seq_t)(n - 1);
    }
}
