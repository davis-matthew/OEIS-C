/*
 * A291359  The arithmetic function u(n,2,5).
 * https://oeis.org/A291359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   u(n,2,5) = 6 for odd n not divisible by 3 or 5, 5 for odd n
 *            divisible by 5 (but not by 3), 3 for odd n divisible by 3, 2 for
 *            even n
 * Range:     u8 n <= 9964, u16 n <= 9964, u32 n <= 9964, u64 n <= 9964,
 *            u128 n <= 9964, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* even n → 2 */
    if ((n & 1ULL) == 0)
        return (seq_t)2;

    /* divisible by 3 → 3 */
    if (n % 3 == 0)
        return (seq_t)3;

    /* divisible by 5 → 5 (but not by 3) */
    if (n % 5 == 0)
        return (seq_t)5;

    /* otherwise → 6 */
    return (seq_t)6;
}
