/*
 * A291338  Number of n X n X n triangular binary arrays symmetric under
 *          rotations of 120 degrees.
 * https://oeis.org/A291338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^ceil(n*(n+1)/6)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the exponent e = ceil(n*(n+1)/6) */
    seq_t nn   = (seq_t)n;
    seq_t num  = nn * (nn + 1);      /* n*(n+1) */
    seq_t e    = (num + 5) / 6;      /* ceil division by 6 */

    /* exponentiation by squaring: 2^e */
    unsigned long long exp = (unsigned long long)e;

    seq_t result = 1;               /* seq_t(1) */
    seq_t base   = 2;               /* seq_t(2) */

    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}
