/*
 * A382156  Partial sums of A382155.
 * https://oeis.org/A382156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1) = 3 a(2) = 9 For n ≥ 3: if n is even (n = 2m),
 *            a(n) = 6m² + 4m − 1 if n is odd (n = 2m+1), a(n) = 6m² + 8m + 1
 * Range:     u8 n <= 12, u16 n <= 208, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;

    if ((n & 1ULL) == 0) {            /* even n = 2m  */
        seq_t m = (seq_t)(n / 2);
        seq_t ans = m * m;
        ans *= (seq_t)6;             /* 6m² */
        ans += m * (seq_t)4;         /* +4m */
        ans -= (seq_t)1;             /* -1 */
        return ans;
    } else {                          /* odd n = 2m+1  */
        seq_t m = (seq_t)((n - 1) / 2);
        seq_t ans = m * m;
        ans *= (seq_t)6;             /* 6m² */
        ans += m * (seq_t)8;         /* +8m */
        ans += (seq_t)1;             /* +1 */
        return ans;
    }
}
