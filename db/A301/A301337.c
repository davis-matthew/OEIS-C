/*
 * A301337  Number of steps required in the worst case for two knights to find
 *          the princess in a castle with n rooms arranged in a line (Castle
 *          and princess puzzle).
 * https://oeis.org/A301337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   for n >= 3, if n mod 6 == 0 then a(n) = 4*(n/6) - 1; if n mod 6
 *            == 1 or 2 then a(n) = 4*floor(n/6); if n mod 6 == 3, 4, or 5
 *            then a(n) = 4*floor(n/6) + 2; additionally, a(1) = 1; a(2) = 1
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)           /* n = 1 or 2 */
        return 1;

    idx_t q = n / 6;     /* floor(n/6) */
    idx_t r = n % 6;     /* n mod 6 */

    seq_t base = (seq_t)q * 4;   /* 4 * floor(n/6) */

    if (r == 0)          /* n ≡ 0 (mod 6) */
        return base - 1;
    if (r == 1 || r == 2)/* n ≡ 1 or 2 (mod 6) */
        return base;
    /* r == 3, 4, or 5 */
    return base + 2;
}
