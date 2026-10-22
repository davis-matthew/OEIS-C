/*
 * A369080  Number of permutations of [n] such that the element sum of each
 *          cycle is odd.
 * https://oeis.org/A369080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1 if n == 1 : a(n) = a(n-1) * 1 else if n even
 *            : a(n) = a(n-1) * (n-1) else if n ≡ 1 (mod 4): a(n) = a(n-1) *
 *            (n+1) else : a(n) = a(n-1) * (n-1) This produces the OEIS
 *            sequence A369080
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t mult;
        if (i == 1) {
            mult = (seq_t)1;
        } else if ((i & 1U) == 0) {    /* even */
            mult = (seq_t)(i - 1);
        } else if ((i & 3U) == 1) {    /* i ≡ 1 (mod 4) */
            mult = (seq_t)(i + 1);
        } else {                       /* i ≡ 3 (mod 4) */
            mult = (seq_t)(i - 1);
        }
        result = result * mult;
    }
    return result;
}
