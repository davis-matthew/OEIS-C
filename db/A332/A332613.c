/*
 * A332613  Covering radius of the dihedral group code D_n.
 * https://oeis.org/A332613
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n - 4 - floor((n-9)/8) (for n ≥ 3)
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 9)
        return (seq_t)n - 3;                     /* n-3 for 3 ≤ n ≤ 8 */
    /* For n ≥ 9: floor((n-9)/8) is simply the unsigned division (n-9)/8 */
    return (seq_t)n - 4 - (seq_t)((n - 9) / 8);
}
