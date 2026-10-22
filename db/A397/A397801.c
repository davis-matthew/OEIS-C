/*
 * A397801  3-adic valuation of the quadruple factorial of n.
 * https://oeis.org/A397801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = sum_{k ≥ 0} v_3(n – 4k), where the terms with n – 4k > 0
 *            are summed; v_3(x) is the 3‑adic valuation of x (the exponent of
 *            the highest power of 3 dividing x). This matches the definition
 *            a(n) = a(n-4) + A007949(n) for n ≥ 4
 * Range:     u8 n <= 2036, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t m = n; m > 0; ) {
        /* compute 3-adic valuation of m */
        seq_t v = 0;
        idx_t t = m;
        while (t % 3 == 0) {
            v++;
            t /= 3;
        }
        total += v;
        if (m <= 4) break;   /* next term would be ≤ 0, stop */
        m -= 4;
    }
    return total;
}
