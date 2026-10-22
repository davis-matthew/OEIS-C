/*
 * A360021  Number of unordered triples of self-avoiding paths with nodes that
 *          cover all vertices of a convex n-gon; one-node paths are allowed.
 * https://oeis.org/A360021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*2^(n-10)*(3^(n-4) + 3*2^(n-3) + 9) for n >=
 *            4 a(1)=0, a(2)=0, a(3)=1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1 || n == 2)
        return (seq_t)0;

    if (n == 3)
        return (seq_t)1;

    /* term1 = n*(n-1)*(n-2) */
    seq_t term1 = (seq_t)n * (seq_t)(n - 1) * (seq_t)(n - 2);

    /* 3^(n-4) */
    seq_t three_pow = (seq_t)1;
    for (idx_t i = 0; i < (n - 4); ++i)
        three_pow *= (seq_t)3;

    /* 2^(n-3) */
    seq_t two_pow = (seq_t)1;
    for (idx_t i = 0; i < (n - 3); ++i)
        two_pow *= (seq_t)2;

    /* inner sum = 3^(n-4) + 3*2^(n-3) + 9 */
    seq_t inner = three_pow;
    inner += two_pow * (seq_t)3;
    inner += (seq_t)9;

    /* multiply term1 with inner */
    seq_t res = term1 * inner;

    /* handle 2^(n-10) factor */
    if (n >= 10) {
        seq_t pow2_part = (seq_t)1;
        for (idx_t i = 0; i < (n - 10); ++i)
            pow2_part *= (seq_t)2;
        res *= pow2_part;
    } else {
        seq_t pow2_part = (seq_t)1;
        for (idx_t i = 0; i < (10 - n); ++i)
            pow2_part *= (seq_t)2;
        res /= pow2_part;   /* exact division */
    }

    return res;
}
