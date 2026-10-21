/*
 * A285951  Positions of 1's in A285949; complement of A285950.
 * https://oeis.org/A285951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A010060(n) + 3*n-1
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A010060(n) is the Thue-Morse sequence: parity of popcount(n-1) */
seq_t a(idx_t n)
{
    /* compute parity of popcount(n-1) */
    unsigned long long p = __builtin_popcountll((unsigned long long)(n - 1));
    seq_t res = (seq_t)n * 3 - 1;   /* 3*n - 1 in seq_t */
    res += (seq_t)(p & 1);          /* add the Thue-Morse bit */
    return res;
}
