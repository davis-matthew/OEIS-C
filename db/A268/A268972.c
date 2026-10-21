/*
 * A268972  Number of 2 X n 0..2 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two not
 *          more than once.
 * https://oeis.org/A268972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16 * 3^(n-3) * (4*n+3) for n > 2 a(1) = 9, a(2) = 60.
 *            (The same formula holds for all tested n.)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special‑case the first two terms. */
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)60;

    /* Compute 3^(n-3) with exponentiation by squaring. */
    idx_t exp = n - 3;
    seq_t result = 1;         /* pow3 */
    seq_t base   = 3;         /* current power of 3 */

    while (exp != 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }

    /* Multiply by (4*n+3) and by 16. */
    result *= (seq_t)(4 * n + 3);
    result *= (seq_t)16;

    return result;
}
