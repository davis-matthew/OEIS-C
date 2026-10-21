/*
 * A251187  Number of (n+2)X(1+2) 0..1 arrays with nondecreasing sum of every
 *          three consecutive values in every row and column.
 * https://oeis.org/A251187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+5)/3)^9 for n ≡ 1 (mod 3); a(n) = (k^3+5k^2+8k+4)^3
 *            for n = 3k; a(n) = (k^3+7k^2+16k+12)^3 for n ≡ 2 (mod 3)
 * Range:     u8 none, u16 n <= 5, u32 n <= 30, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)  /* offset starts at 1, but guard against 0 */
        return 0;

    idx_t r = n % 3ULL;
    seq_t result;
    if (r == 1) {                 /* n = 3k+1  →  t = (n+5)/3  (integer)   */
        idx_t t = (n + 5) / 3;    /* t = k+2                                 */
        seq_t T = (seq_t)t;
        seq_t f = T * T * T;      /* f = t³                                  */
        result = f * f * f;       /* a = f³ = t⁹                              */
    }
    else if (r == 0) {            /* n = 3k  →  k = n/3                     */
        idx_t k = n / 3;
        seq_t K = (seq_t)k;
        /* f = k³ + 5k² + 8k + 4 */
        seq_t f = K * K * K + 5 * K * K + 8 * K + 4;
        result = f * f * f;       /* a = f³                                  */
    }
    else { /* r == 2, n = 3k+2 → k = (n-2)/3 */
        idx_t k = (n - 2) / 3;
        seq_t K = (seq_t)k;
        /* f = k³ + 7k² + 16k + 12 */
        seq_t f = K * K * K + 7 * K * K + 16 * K + 12;
        result = f * f * f;       /* a = f³                                  */
    }
    return result;
}
