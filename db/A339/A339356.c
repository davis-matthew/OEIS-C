/*
 * A339356  Maximum number of copies of a 123456 permutation pattern in an
 *          alternating (or zig-zag) permutation of length n + 9.
 * https://oeis.org/A339356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   For n odd: a(2k-1) = 16*A259181(k) =
 *            2k(k+1)(k+2)(k+3)(2k^2+6k+7)/45 For n even: a(2k) = 64*C(k+5,6)
 *            - 32*C(k+4,5) where C(n,r) is the binomial coefficient
 * Range:     u8 none, u16 n <= 6, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: compute C(n,r) for idx_t arguments using seq_t arithmetic. */
static seq_t binom_idx(idx_t n, idx_t r)
{
    if (r > n) return (seq_t)0;
    if (r > n - r) r = n - r;    /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= r; ++i) {
        res = res * (seq_t)(n - r + i);   /* numerator term */
        res = res / (seq_t)i;             /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used – sequence starts at 1 */

    if (n & 1) { /* odd n = 2k-1 */
        idx_t k = (n + 1) >> 1;          /* k = (n+1)/2 */
        seq_t term1 = (seq_t)2 * k;
        seq_t term2 = (seq_t)(k + 1);
        seq_t term3 = (seq_t)(k + 2);
        seq_t term4 = (seq_t)(k + 3);
        seq_t poly   = (seq_t)(2*k*k + 6*k + 7);
        seq_t prod   = term1 * term2 * term3 ;    /* keep intermediate size small */
        prod = prod * term4;
        prod = prod * poly;
        return prod / (seq_t)45;
    } else { /* even n = 2k */
        idx_t k = n >> 1;                 /* k = n/2 */
        seq_t c1 = binom_idx(k + 5, 6);
        seq_t c2 = binom_idx(k + 4, 5);
        return (seq_t)64 * c1 - (seq_t)32 * c2;
    }
}
