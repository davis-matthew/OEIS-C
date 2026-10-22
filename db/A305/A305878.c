/*
 * A305878  For any number n >= 0: apply the map 0 -> "0", 1 -> "01", 2 ->
 *          "011" to the ternary representation of n and interpret the result
 *          as a binary string.
 * https://oeis.org/A305878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0 a(3*n) = 2*a(n) a(3*n+1) = 4*a(n)+1 a(3*n+2) = 8*a(n)+3
 * Range:     u8 n <= 43, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t q = n / 3;
    int r = n % 3;          /* r is 0,1 or 2 */
    seq_t sub = a(q);       /* recursive call on the higher part */

    if (r == 0) return sub << 1;           /* *2   */
    if (r == 1) return (sub << 2) | 1;     /* *4 + 1 */
    /* r == 2 */
    return (sub << 3) | 3;                 /* *8 + 3 */
}
