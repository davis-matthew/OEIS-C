/*
 * A367171  The sum of divisors of the largest unitary divisor of n that is a
 *          term of A138302.
 * https://oeis.org/A367171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π over prime powers p^e dividing n of * (p^(e+1)-1)/(p-1)
 *            if e ∈ {1,2,4} (i.e. sigma(p^e)) * 1 otherwise (since the only
 *            unitary divisor that is * a term of A138302 is 1 in this case) *
 *            This implements the multiplicative definition of A367171. * The
 *            function works for n ≥ 1 and uses only i...
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* a(1) = 1 */
    if (n == 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t nn = n;

    for (idx_t p = 2; p * p <= nn; ++p) {
        if (nn % p == 0) {
            /* count exponent e of prime p */
            idx_t e = 0;
            while (nn % p == 0) {
                ++e;
                nn /= p;
            }

            seq_t factor;
            if (e == 1) {
                factor = (seq_t)p + 1;                      /* sigma(p)   */
            } else if (e == 2) {
                seq_t p2 = (seq_t)p * p;
                seq_t p3 = p2 * (seq_t)p;                  /* p^3 */
                factor = (p3 - (seq_t)1) / ((seq_t)p - 1); /* (p^3-1)/(p-1) */
            } else if (e == 4) {
                seq_t p2 = (seq_t)p * p;
                seq_t p4 = p2 * p2;                       /* p^4 */
                seq_t p5 = p4 * (seq_t)p;                  /* p^5 */
                factor = (p5 - (seq_t)1) / ((seq_t)p - 1); /* (p^5-1)/(p-1) */
            } else {
                /* e not in {1,2,4} → only unitary divisor 1 is a term */
                factor = (seq_t)1;
            }

            result *= factor;
        }
    }

    /* Remaining prime factor (if any) has exponent 1 */
    if (nn > 1) {
        result *= ((seq_t)nn + (seq_t)1);                   /* sigma(nn) */
    }

    return result;
}
