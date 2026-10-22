/*
 * A378959  Sum, in base 10, of the permutations of the digits of n, written
 *          in base 2.
 * https://oeis.org/A378959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = C(k-1, p-1) * (2^k - 1) * where k = floor_log2(n) + 1 is
 *            the number of bits of n (for n > 0) * and p = popcount(n) is the
 *            number of 1‑bits in n
 * Range:     u8 n <= 32, u16 n <= 518, u32 n <= 9613, u64 n <= 9613,
 *            u128 n <= 9613, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* count 1-bits (p) */
    idx_t temp = n;
    idx_t p = 0;
    while (temp) {
        ++p;
        temp &= temp - 1;         /* remove lowest set bit */
    }

    /* count bits (k) */
    temp = n;
    idx_t k = 0;
    while (temp) {
        ++k;
        temp >>= 1;
    }

    /* compute C(k-1, p-1) */
    seq_t comb = 1;
    idx_t r = p - 1;
    for (idx_t i = 1; i <= r; ++i) {
        comb = comb * (seq_t)(k - i) / (seq_t)i;
    }

    /* compute 2^k - 1 */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < k; ++i) {
        pow2 = pow2 * 2;
    }
    pow2 = pow2 - 1;

    /* final sum */
    return comb * pow2;
}
