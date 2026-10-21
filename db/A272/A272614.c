/*
 * A272614  Numbers whose binary digits, except for the first "1", are given
 *          by floor(((k-n)/n) mod 2) with 1<=k<=n.
 * https://oeis.org/A272614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2^n + Σ_{k=1..n}[floor(((n-k)/k) mod 2) * 2^(n-k)]
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t    result = (seq_t)0;
    seq_t    pow2   = (seq_t)1;   /* 2^0 */

    for (idx_t p = 0; p <= n; ++p) {
        if (p > 0) {
            pow2 <<= 1;          /* pow2 = 2^p now */
        }

        if (p < n) {
            idx_t k = n - p;     /* k runs from 1 to n */
            idx_t t = p / k;     /* floor((n-k)/k) */
            if (t & (idx_t)1) {
                result += pow2;   /* add 2^p when coefficient is 1 */
            }
        } else { /* p == n : leading '1' bit */
            result += pow2;
        }
    }
    return result;
}
