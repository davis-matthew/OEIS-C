/*
 * A349115  a(n) = 8^n * P(n, 3*n), where P(n, x) is n-th Legendre polynomial.
 * https://oeis.org/A349115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8^n * P(n, 3*n) (Legendre polynomial of degree n at 3n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* 3*n as seq_t */
    seq_t three_n = (seq_t)(3 * n);

    /* binomial coefficient C(2n, n) = (2n)!/(n!)^2   computed multiplicatively */
    seq_t bin = 1;
    for (idx_t i = 1; i <= n; ++i) {
        bin = bin * (seq_t)(n + i);
        bin = bin / (seq_t)i;
    }

    /* (3n)^n */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow3 = pow3 * three_n;

    /* first term magnitude t_0 = C(2n,n) * (3n)^n */
    seq_t t   = bin * pow3;
    seq_t sum = t;                /* accumulates the sum with signs */

    idx_t maxk = n / 2;             /* floor(n/2) */
    for (idx_t k = 0; k < maxk; ++k) {
        /* numerator = (n-k)*(n-2k)*(n-2k-1) */
        seq_t n_minus_k  = (seq_t)(n - k);
        seq_t n_minus_2k = (seq_t)(n - 2 * k);
        seq_t num = n_minus_k * n_minus_2k * (n_minus_2k - 1);

        /* denominator = (k+1)*(2n-2k)*(2n-2k-1) */
        seq_t k_plus_1       = (seq_t)(k + 1);
        seq_t two_n_minus_2k = (seq_t)(2 * n - 2 * k);
        seq_t den = k_plus_1 * two_n_minus_2k * (two_n_minus_2k - 1);

        /* t_next = t * num / den / (3n)^2   (absolute value) */
        seq_t t_next = t * num / den;
        seq_t three_n_sq = three_n * three_n;
        t_next = t_next / three_n_sq;

        t = t_next;

        /* add or subtract the next term depending on its sign */
        if (((k + 1) & 1) == 0) /* even index → positive */
            sum = sum + t;
        else                    /* odd index  → negative */
            sum = sum - t;
    }

    /* multiply by 4^n (since 8^n / 2^n = 4^n) */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow4 = pow4 * (seq_t)4;

    return pow4 * sum;
}
