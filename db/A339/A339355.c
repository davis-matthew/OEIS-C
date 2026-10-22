/*
 * A339355  Maximum number of copies of a 12345 permutation pattern in an
 *          alternating (or zig-zag) permutation of length n + 7.
 * https://oeis.org/A339355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 32*binomial(n/2+4,5) - 16*binomial(n/2+3,4) if n even
 *            (4*((n+1)/2)*(((n+1)/2)^4 + 5*((n+1)/2)^3 + 10*((n+1)/2)^2 +
 *            10*((n+1)/2) + 4)) / 15 if n odd
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* binomial(n,5) for n >= 5 */
static inline seq_t binom5(idx_t n)
{
    if (n < 5) return 0;
    seq_t r = 1;
    for (idx_t i = 1; i <= 5; ++i) {
        r = r * (seq_t)(n - 5 + i) / i;
    }
    return r;
}

/* binomial(n,4) for n >= 4 */
static inline seq_t binom4(idx_t n)
{
    if (n < 4) return 0;
    seq_t r = 1;
    for (idx_t i = 1; i <= 4; ++i) {
        r = r * (seq_t)(n - 4 + i) / i;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not part of the sequence */

    if ((n & 1U) == 0) {           /* even index */
        idx_t m = n >> 1;          /* m = n/2  */
        seq_t bin5 = binom5(m + 4);/* C(m+4,5) */
        seq_t bin4 = binom4(m + 3);/* C(m+3,4) */
        return (seq_t)32 * bin5 - (seq_t)16 * bin4;
    } else {                       /* odd index  */
        idx_t k = (n + 1) >> 1;    /* k = (n+1)/2 */
        seq_t k_seq = (seq_t)k;
        seq_t k2 = k_seq * k_seq;
        seq_t k3 = k2 * k_seq;
        seq_t k4 = k3 * k_seq;
        seq_t poly = k4
                   + (seq_t)5 * k3
                   + (seq_t)10 * k2
                   + (seq_t)10 * k_seq
                   + (seq_t)4;
        seq_t temp = k_seq * poly;          /* = k * poly   */
        seq_t div  = temp / 15;
        seq_t rem  = temp % 15;
        return div * 4 + (rem * 4) / 15;
    }
}
