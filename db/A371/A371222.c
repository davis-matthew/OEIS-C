/*
 * A371222  Product of digits of (n written in base 3) mod 3.
 * https://oeis.org/A371222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of digits of n written in base 3 modulo 3
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* base‑3 digit 0 → product 0 */

    unsigned int twos = 0;

    while (n > 0) {
        idx_t digit = n % 3;
        if (digit == 0) return 0;  /* any zero digit zeros the product */
        if (digit == 2) twos++;    /* count how many 2's we see   */
        n /= 3;
    }

    /* product = 2^twos (mod 3) → 1 if even number of twos, 2 if odd */
    return (twos & 1) ? 2 : 1;
}
