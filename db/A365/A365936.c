/*
 * A365936  Final digit (in decimal system) of n^(n-1) = A000169(n).
 * https://oeis.org/A365936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n^(n-1) mod 10
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base modulo 10 */
    seq_t base = (seq_t)(n % 10);
    seq_t result = 1 % 10;            /* start with 1 mod 10 */
    idx_t exp = n - 1;

    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % 10;
        base = (base * base) % 10;
        exp >>= 1;
    }

    return result;
}
