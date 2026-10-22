/*
 * A396574  a(n) is the smallest number k such that n divides binomial(k,m)
 *          for 1 <= m <= n-1.
 * https://oeis.org/A396574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = n * Product_{prime p | n} p^floor(log_p(n-1)), n > 1;
 *            a(1) = 1
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 209, u64 n <= 2729,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    seq_t result = (seq_t)n;         /* start with n */
    idx_t   m      = n - 1;           /* n-1 for the exponent calculations */
    idx_t   nn     = n;               /* copy of n for factorization */

    for (idx_t p = 2; p * p <= nn; ++p) {
        if (nn % p == 0) {
            /* compute p^floor(log_p(m)) */
            seq_t pow   = 1;
            seq_t p_seq = (seq_t)p;
            while (pow * p_seq <= (seq_t)m) {
                pow *= p_seq;
            }
            result *= pow; /* multiplier may be 1 */

            /* remove all copies of p */
            while (nn % p == 0) nn /= p;
        }
    }

    /* remaining prime factor, if any */
    if (nn > 1) {
        /* For a remaining prime nn, floor(log_nn(m)) is 0 if nn > m, so multiplier 1 */
        seq_t pow   = 1;
        seq_t p_seq = (seq_t)nn;
        while (pow * p_seq <= (seq_t)m) {
            pow *= p_seq;
        }
        result *= pow;
    }

    return result;
}
