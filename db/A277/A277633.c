/*
 * A277633  Number of aperiodic necklaces (Lyndon words) with k<=8 black beads
 *          and n-k white beads.
 * https://oeis.org/A277633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{8} (1/n) * Σ_{d | gcd(n,k)} μ(d) * C(n/d, k/d)
 *            (where μ is the Möbius function and C is the binomial
 *            coefficient; C(a,b) = 0 if b > a). For n=0 the value is defined
 *            to be 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t my_gcd(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Möbius function for small arguments (≤ 10000).  */
static inline int mobius_small(idx_t m) {
    if (m == 1) return 1;
    int cnt = 0;
    /* trial division up to sqrt(m) */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int c = 0;
            while (m % p == 0) {
                m /= p;
                ++c;
                if (c > 1) return 0;   /* square factor */
            }
            ++cnt;
        }
    }
    if (m > 1) ++cnt;      /* remaining prime factor */
    return (cnt & 1) ? -1 : 1;
}

/* Binomial coefficient C(n,k) for 0 ≤ k ≤ n ≤ 10000, computed multiplicatively. */
static inline seq_t binom_small(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;   /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)n - k + i);  /* multiply numerator terms */
        res = res / (seq_t)i;            /* divide by denominator term */
    }
    return res;
}

seq_t a(idx_t n) {
    /* handle the offset */
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    seq_t total = 0;
    for (idx_t k = 0; k <= 8; ++k) {
        /* compute gcd(n,k) */
        idx_t g = my_gcd(n, k);

        seq_t sumMuBin = 0;

        /* iterate over all divisors d of g */
        for (idx_t d = 1; d <= g; ++d) {
            if (g % d != 0) continue;
            int mu = mobius_small(d);
            /* binomial coefficient C(n/d, k/d) */
            seq_t bin = binom_small(n / d, k / d);
            sumMuBin = sumMuBin + (seq_t)mu * bin;
        }

        /* N(n,k) = sumMuBin / n  (n > 0) */
        seq_t Nk = sumMuBin / (seq_t)n;
        total = total + Nk;
    }
    return total;
}
