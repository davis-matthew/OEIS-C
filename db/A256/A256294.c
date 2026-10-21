/*
 * A256294  Apply the transformation 0 -> 1 -> 2 -> 3 -> 0 to the digits of n
 *          written in base 4, then convert back to base 10.
 * https://oeis.org/A256294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ∑_{i≥0} ((d_i+1) mod 4)·4^i, where d_i are the base‑4
 *            digits of n. Transform each base‑4 digit by adding 1 modulo 4,
 *            then evaluate the new base‑4 number
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;               /* 0 (base4) → 1 (base4) → 1 (decimal) */

    seq_t result = 0;
    seq_t pow4   = 1;           /* 4^0 initially */
    idx_t tmp    = n;

    while (tmp > 0) {
        idx_t digit     = tmp % 4;          /* current base‑4 digit of n   */
        seq_t newdigit  = (digit + 1) & 3;  /* (d+1) mod 4 in seq_t */

        result += newdigit * pow4;          /* accumulate the transformed value */
        pow4 <<= 2;                         /* multiply by 4 for next digit   */
        tmp   /= 4;
    }
    return result;
}
