/*
 * A244416  6-adic value of 1/n for n >= 1.
 * https://oeis.org/A244416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = 1 if n ≡ 1 or 5 (mod 6) * a(n) = 6^v2(n) if n ≡ 2 or 4
 *            (mod 6) * a(n) = 6^v3(n) if n ≡ 3 (mod 6) * a(n) = 6^max(v2(n),
 *            v3(n)) if n ≡ 0 (mod 6) * where v2(n) = exponent of 2 in the
 *            factorisation of n * v3(n) = exponent of 3 in the factorisation
 *            of n
 * Range:     u8 n <= 15, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n % 6;                       /* n mod 6 */
    if (k == 1 || k == 5)
        return 1;

    /* Count exponent of 2 (v2) */
    idx_t temp = n;
    idx_t v2 = 0;
    while ((temp & 1) == 0) {
        v2++;
        temp >>= 1;
    }

    /* Count exponent of 3 (v3) */
    temp = n;
    idx_t v3 = 0;
    while (temp % 3 == 0) {
        v3++;
        temp /= 3;
    }

    idx_t exp;
    if (k == 0)               /* n divisible by 6 */
        exp = v2 > v3 ? v2 : v3;
    else if (k == 2 || k == 4)
        exp = v2;
    else                      /* k == 3 */
        exp = v3;

    seq_t result = 1;
    while (exp > 0) {
        result *= (seq_t)6;
        exp--;
    }
    return result;
}
