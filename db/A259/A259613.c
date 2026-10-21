/*
 * A259613  a(n) = binomial(6*n,2*n)/3, n>0, a(0)=1.
 * https://oeis.org/A259613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(6*n, 2*n)/3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Euclidean algorithm for seq_t */
static seq_t my_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;                       /* a(0) = 1 by definition */

    idx_t k = (idx_t)2 * n;                    /* number of terms in the product */
    seq_t result = (seq_t)1;
    seq_t four_n = (seq_t)4 * (seq_t)n;        /* 4*n as seq_t */

    for (idx_t i = (idx_t)1; i <= k; ++i) {
        seq_t num = four_n + (seq_t)i;            /* numerator: 4*n + i */
        seq_t den = (seq_t)i;                    /* denominator: i */

        /* cancel common factors between numerator and denominator */
        seq_t g = my_gcd(num, den);
        num /= g;
        den /= g;

        /* cancel common factors between current result and remaining denominator */
        g = my_gcd(result, den);
        result /= g;
        den /= g;

        /* after reductions den should be 1 */
        result *= num;
    }

    /* final division by 3 */
    result /= (seq_t)3;
    return result;
}
