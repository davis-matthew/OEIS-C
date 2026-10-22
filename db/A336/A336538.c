/*
 * A336538  G.f. A(x) satisfies A(x) = 1 + x * A(x)^3 * (2 + A(x)).
 * https://oeis.org/A336538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 0 a(n) = (1/n) * Sum_{k=1..n} 3^k *
 *            binomial(n,k) * binomial(3*n, k-1) for n > 0 (OEIS A336538)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 0 -> first term */
    if (n == 0) return 1;

    /* working variables of type seq_t */
    seq_t sum = 0;        /* running sum of the summand */
    seq_t b = 1;          /* binomial(n, k-1)  (starts with nC0 = 1) */
    seq_t c = 1;          /* binomial(3*n, k-2) (starts with (3n)C0 = 1) */
    seq_t p3 = 1;         /* 3^k  (starts with 3^0 = 1) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* update b = nCk  from nC(k-1) */
        b = (b * (seq_t)(n - (k - 1))) / (seq_t)k;

        /* update c = (3*n)C(k-1)  from (3*n)C(k-2) */
        if (k > 1)
        {
            c = (c * (seq_t)(3 * n - (k - 2))) / (seq_t)(k - 1);
        }

        /* update power of 3: 3^k from 3^(k-1) */
        p3 *= 3;

        /* term = binomial(n,k) * binomial(3*n, k-1) * 3^k */
        seq_t term = b * c * p3;

        sum += term;
    }

    /* divide by n to obtain a(n) */
    return sum / (seq_t)n;
}
