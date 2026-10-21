/*
 * A288770  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 505", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 For odd n: a(n) = 2^(n+1) - 2 For even n ≥ 2: Let m =
 *            n/2 and k = floor(log2(m+1)) Then a(n) = 2^(2^k) - 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow_two(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r *= (seq_t)2;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if (n & 1U) { /* odd n */
        idx_t exp = n + 1;
        return pow_two(exp) - (seq_t)2;
    } else { /* even n ≥ 2 */
        idx_t m = n >> 1;        /* n/2 */
        idx_t inc = m + 1;
        idx_t k = 0;
        while (inc > 1) {
            inc >>= 1;
            ++k;
        }
        idx_t exponent = (idx_t)1 << k;   /* 2^k */
        return pow_two(exponent) - (seq_t)1;
    }
}
