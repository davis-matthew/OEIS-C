/*
 * A317842  Dirichlet convolution of A284013 (= n - A002487(n)) with itself,
 *          where A002487 is Stern's Diatomic sequence.
 * https://oeis.org/A317842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d - A002487(n/d)) * (d - A002487(d)) where
 *            A002487 is Stern's diatomic sequence, S(0)=0, S(1)=1,
 *            S(2k)=S(k), S(2k+1)=S(k)+S(k+1)
 * Range:     u8 n <= 71, u16 n <= 2519, u32 n <= 9554, u64 n <= 9554,
 *            u128 n <= 9554, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes Stern's diatomic sequence for a given n */
static seq_t stern(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if ((n & 1) == 0)          /* even */
        return stern(n >> 1);
    /* odd: (2k+1) => k = n>>1 */
    return stern(n >> 1) + stern((n >> 1) + 1);
}

/* Main sequence function: a(n) = Σ_{d|n} (n/d - S(n/d)) * (d - S(d)) */
seq_t a(idx_t n) {
    seq_t result = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* divisor d = i */
            idx_t d = i;
            idx_t nd = n / d;

            seq_t val_d   = (seq_t)d;
            seq_t val_n_d = (seq_t)nd;
            seq_t Sd      = stern(d);
            seq_t Snd     = stern(nd);

            seq_t term1 = (val_d - Sd) * (val_n_d - Snd);
            result += term1;

            /* the complementary divisor d2 = n/i, if distinct */
            if (i != nd) {
                idx_t d2 = nd;
                idx_t nd2 = n / d2; /* should equal d */

                seq_t val_d2   = (seq_t)d2;
                seq_t val_n_d2 = (seq_t)nd2;
                seq_t Sd2      = stern(d2);
                seq_t Snd2     = stern(nd2);

                seq_t term2 = (val_d2 - Sd2) * (val_n_d2 - Snd2);
                result += term2;
            }
        }
    }
    return result;
}
