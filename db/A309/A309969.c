/*
 * A309969  a(n) is the least base b in the range 2..n where the product of
 *          the digits of n is maximized.
 * https://oeis.org/A309969
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = least base b (2 ≤ b ≤ n) that maximizes the product of
 *            the digits of n when n is written in base b
 * Range:     u8 n <= 383, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) {
        return 0;
    }

    seq_t best_prod = 0;
    idx_t best_base = 2;

    for (idx_t b = 2; b <= n; ++b) {
        seq_t prod = 1;
        idx_t tmp = n;

        while (tmp) {
            idx_t digit = tmp % b;
            prod *= (seq_t)digit;
            if (prod == 0) {
                break; /* remaining digits cannot raise the product */
            }
            tmp /= b;
        }

        if (prod > best_prod) {
            best_prod = prod;
            best_base = b;
        }
    }

    return (seq_t)best_base;
}
