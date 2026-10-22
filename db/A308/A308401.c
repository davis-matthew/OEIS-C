/*
 * A308401  Number of bracelets (turnover necklaces) of length n that have no
 *          reflection symmetry and consist of 6 white beads and n-6 black
 *          beads.
 * https://oeis.org/A308401
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -(1/2)*C(floor(n/2),3) + (1/12)*∑_{d|gcd(n,6)}
 *            φ(d)*C((n/d)-1,(6/d)-1)
 * Range:     u8 n <= 11, u16 n <= 26, u32 n <= 226, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* binomial(n,k) – fast multiplicative version for small k (≤5) */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = (res * (n - k + i)) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 9 (offset 9).  Return 0 for smaller n. */
    if (n < 9)
        return 0;

    /* Compute gcd(n,6) – only four possibilities: 1,2,3,6. */
    idx_t g;
    if ((n & 1) == 0) {               /* n even */
        g = (n % 3 == 0) ? 6 : 2;
    } else {                           /* n odd */
        g = (n % 3 == 0) ? 3 : 1;
    }

    /* Summation over the divisors of g.  1,2,3,6 are the only candidates. */
    static const idx_t divs[4] = {1, 2, 3, 6};
    seq_t sum = 0;
    for (int i = 0; i < 4; ++i) {
        idx_t d = divs[i];
        if (g % d != 0)
            continue;

        idx_t nd = n / d;          /* value of n/d */
        idx_t kd = 6 / d;          /* value of 6/d */
        seq_t bin = binom_idx(nd - 1, kd - 1);

        /* Euler's totient φ(d) – only 1 or 2 for these small d. */
        seq_t phi = (d == 1 || d == 2) ? 1 : 2;   /* φ(3)=2, φ(6)=2 */

        sum += phi * bin;
    }

    /* (1/12) * sum – integer division truncates the inevitable .5 correctly. */
    seq_t part1 = sum / 12;

    /* Second part: -(1/2) * C(floor(n/2), 3).  Binomial will always be even
       for the arguments that occur, so division by 2 is exact (integer). */
    idx_t k = n >> 1;              /* floor(n/2) */
    seq_t bin2 = binom_idx(k, 3);
    seq_t half = bin2 / 2;

    return part1 - half;
}
