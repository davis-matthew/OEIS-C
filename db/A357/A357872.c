/*
 * A357872  a(n) = n * (3/2)^(v(n, 2) - v(n, 3)) where v(n, k) = valuation(n,
 *          k) mod 2 for n > 0.
 * https://oeis.org/A357872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * (3/2)^(v(n,2) - v(n,3)) where v(n,p)=valuation(n,p)
 *            mod 2
 * Range:     u8 n <= 177, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS sequence A357872. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n = 0, return 0 safely */

    seq_t result = (seq_t)n;

    /* Count the exponent of 2 in n. */
    idx_t e2 = 0;
    idx_t temp = n;
    while ((temp & 1) == 0) {
        temp >>= 1;
        ++e2;
    }

    /* Count the exponent of 3 in n. */
    idx_t e3 = 0;
    while (temp % 3 == 0) {
        temp /= 3;
        ++e3;
    }

    /* Parity of the exponents. */
    int r = e2 & 1;   /* e2 mod 2 */
    int s = e3 & 1;   /* e3 mod 2 */

    if (r == 1 && s == 0) {        /* multiply by 3/2 */
        result /= 2;
        result *= 3;
    } else if (r == 0 && s == 1) { /* multiply by 2/3 */
        result /= 3;
        result *= 2;
    }                               /* else r==s => multiply by 1 */

    return result;
}
