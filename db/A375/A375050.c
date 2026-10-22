/*
 * A375050  Obverse convolution (n(n+1)/2)**(n(n+1)/2); see Comments.
 * https://oeis.org/A375050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n*(n+1)/2) * Π_{j=0}^{n-1} [ (1 - n/2 + j)^2 + n(n+2)/4
 *            ]
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from a(n) = (n*(n+1)/2) * [1-(n+s)/2]_n * [1-(n-s)/2]_n with s = sqrt(-n*(2+n)) */
seq_t a(idx_t n)
{
    seq_t T = (seq_t)n * ((seq_t)n + 1) / 2;
    seq_t prod = 1;
    for (idx_t j = 0; j < n; ++j) {
        idx_t m = j + 1;                     /* m = 1,2,…,n */
        long long diff = (long long)2 * (long long)m - (long long)n;
        long long sq   = diff * diff;
        long long part = (long long)n * ((long long)n + 2);
        long long num  = sq + part;          /* always divisible by 4 */
        seq_t term = (seq_t)(num / 4);
        prod *= term;
    }
    return T * prod;
}
