/*
 * A277631  Number of aperiodic necklaces (Lyndon words) with k<=6 black beads
 *          and n-k white beads.
 * https://oeis.org/A277631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0, a(1) = 2, for n > 1: a(n) =
 *            Σ_{k=1}^{min(6,n)} (1/n) * Σ_{d | gcd(n,k)} μ(d) * C(n/d, k/d)
 *            where μ(1)=1, μ(2)=μ(3)=μ(5)=-1, μ(6)=1, μ(other)=0
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static inline seq_t binom_seq(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = (res * (n - k + i)) / i;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    seq_t total = 0;
    idx_t maxk = n < 6 ? n : 6;
    for (idx_t k = 1; k <= maxk; ++k) {
        idx_t g = gcd_idx(n, k);   /* g <= 6 for k>0 */
        seq_t pos = 0, neg = 0;
        for (idx_t d = 1; d <= g; ++d) {
            if (g % d != 0) continue;
            idx_t m  = n / d;
            idx_t kk = k / d;
            seq_t bin = binom_seq(m, kk);
            if (d == 1) pos += bin;
            else if (d == 2 || d == 3 || d == 5) neg += bin;
            else if (d == 6) pos += bin;
        }
        total += pos - neg;
    }
    return total / (seq_t)n;
}
