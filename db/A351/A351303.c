/*
 * A351303  a(n) = n^8 * Product_{p|n, p prime} (1 + 1/p^8).
 * https://oeis.org/A351303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n^8 * Product_{p|n} (1 + 1/p^8)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence is multiplicative with a(p^e) = p^(8e) + p^(8e-8)   */
seq_t a(idx_t n) {
    if (n <= 1) return 1;          /* a(1) = 1 */
    seq_t result = 1;
    idx_t m = n;

    /* trial division factorisation of m */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }

            /* compute (p^8)^e  and  (p^8)^(e-1) */
            seq_t basep8 = 1;          /* p^8  */
            for (int i = 0; i < 8; ++i) basep8 *= (seq_t)p;

            seq_t p8e = 1;             /* (p^8)^e   */
            for (int i = 0; i < e; ++i) p8e *= basep8;

            seq_t p8e_minus8 = 1;      /* (p^8)^(e-1), equals 1 when e==1   */
            for (int i = 0; i < e - 1; ++i) p8e_minus8 *= basep8;

            seq_t part = p8e + p8e_minus8;  /* contribution of p^e   */
            result *= part;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        idx_t p = m;                  /* m is prime now */
        seq_t basep8 = 1;
        for (int i = 0; i < 8; ++i) basep8 *= p;  /* p^8              */
        seq_t part = basep8 + 1;      /* p^8 + 1 = a(p)  */
        result *= part;
    }

    return result;
}
