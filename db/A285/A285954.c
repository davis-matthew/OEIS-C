/*
 * A285954  Positions of 1 in A285952; complement of A285953.
 * https://oeis.org/A285954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n+1) = 3n + 1 a(2n) = 3n - A010060(2n) where A010060(k) =
 *            parity of popcount(k-1) (thue‑morse, 1‑indexed)
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1, offset is 1 */
    if (n & 1U) {                    /* odd n :  n = 2m+1  */
        idx_t m = (n - 1U) >> 1;     /* m = (n-1)/2  */
        return (seq_t)3 * (seq_t)m + (seq_t)1;
    } else {                          /* even n : n = 2m  */
        seq_t half = (seq_t)(n >> 1);          /* m = n/2      */
        /* parity of popcount(n-1)  (t( n )) */
        uint64_t u = (uint64_t)n - 1ULL;
        int par = __builtin_parityll(u);       /* 0 or 1       */
        return (seq_t)3 * half - (seq_t)par;
    }
}
