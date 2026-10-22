/*
 * A351990  Number of minimum edge covers of the complete graph K_n.
 * https://oeis.org/A351990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(2) = 1. For even n: a(n) = (n-1)!! (= product of odd
 *            numbers 1·3·5···(n-1)). For odd n: a(n) = (n!)/ (2^((n-1)/2) ·
 *            ((n-1)/2–1)!) = (k)(k+1)…(2k+1) / 2^k with n = 2k+1. The
 *            odd‑case value is always an integer
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 18, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* Even n: product of odd numbers up to n-1 */
    if ((n & 1U) == 0) {
        idx_t k = n >> 1;          /* k = n/2  */
        seq_t res = 1;
        for (idx_t i = 1; i <= k; ++i) {
            res *= (seq_t)(2 * i - 1);
        }
        return res;
    }

    /* Odd n:  n = 2k+1 (k >= 1) */
    idx_t k = (n - 1) >> 1;   /* k = (n-1)/2  */
    seq_t res = 1;
    idx_t rem = k;            /* number of 2’s that must be removed  */

    for (idx_t i = k; i <= n; ++i) {
        seq_t v = (seq_t)i;

        /* Remove as many 2’s as possible, but not more than 'rem'. */
        while (rem > 0 && (v & (seq_t)1U) == 0) {
            v >>= 1;
            --rem;
        }

        res *= v;
    }

    /* rem should be zero here because the division by 2^k is exact. */
    return res;
}
