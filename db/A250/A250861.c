/*
 * A250861  Number of (n+2) X (1+2) 0..3 arrays with nondecreasing sum of
 *          every three consecutive values in every row and column.
 * https://oeis.org/A250861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (C(l0+3,3) * C(l1+3,3) * C(l2+3,3))^3 where m = n + 2, l0
 *            = (m + 2) / 3, l1 = (m + 1) / 3, l2 = m / 3 C(k,3) =
 *            k*(k-1)*(k-2)/6 (k >= 3)
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 20, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes binomial coefficient C(k,3) for k >= 3 using seq_t arithmetic. */
static seq_t binom3(seq_t k)
{
    seq_t res = k * (k - 1); /* k*(k-1) */
    res /= 2;                /* divide by 2 (factor of 6) */
    res = res * (k - 2);     /* multiply by (k-2) */
    res /= 3;                /* divide by 3 (remaining factor of 6) */
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; a(0) is undefined */
    /* total number of columns */
    idx_t m = n + 2;

    /* lengths of the three residue classes modulo 3 */
    idx_t l0 = (m + 2) / 3;
    idx_t l1 = (m + 1) / 3;
    idx_t l2 =  m      / 3;

    /* compute each binomial C(l_i + 3, 3) */
    seq_t k0 = (seq_t)l0 + 3;
    seq_t k1 = (seq_t)l1 + 3;
    seq_t k2 = (seq_t)l2 + 3;

    seq_t b0 = binom3(k0);
    seq_t b1 = binom3(k1);
    seq_t b2 = binom3(k2);

    /* product of the three binomials (number of ways to fill one row) */
    seq_t prod = b0 * b1;
    prod = prod * b2;

    /* total number of matrices: prod^3 */
    seq_t total = prod * prod;
    total = total * prod;

    return total;
}
