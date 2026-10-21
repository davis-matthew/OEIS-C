/*
 * A277619  Number of aperiodic necklaces (Lyndon words) with k<=4 black beads
 *          and n-k white beads.
 * https://oeis.org/A277619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0..4} (1/n) * sum_{d | gcd(n,k)} μ(d) * binom(n/d,
 *            k/d) where μ is the Möbius function and binom is the binomial
 *            coefficient
 * Range:     u8 n <= 18, u16 n <= 116, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Euclidean gcd for idx_t (32 or 64‑bit unsigned) */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Möbius for d <= 4 */
static int mobius_small(int d)
{
    switch (d) {
        case 1: return 1;
        case 2: return -1;
        case 3: return -1;
        case 4: return 0;
        default: return 0; /* should not happen for d <= 4 */
    }
}

/* Small binomial: binom(n, k) for 0 <= k <= 4 */
static long long small_binom(int n, int k)
{
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    long long res = 1;
    /* use symmetrical small k to reduce multiplications */
    if (k > n - k) k = n - k;
    for (int i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Offset 0: a(0) = 1 */
    if (n == 0) return (seq_t)1;

    long long total = 0;           /* accumulates numerator of recurrence */

    /* Handle k = 0 separately (only n = 1 gives a primitive word) */
    if (n == 1) total += 1;        /* L(1,0) = 1 */

    /* k = 1..4 */
    for (int k = 1; k <= 4; ++k) {
        int g = (int)gcd_idx(n, (idx_t)k);   /* gcd(n,k) is at most 4 */
        /* enumerate all divisors of g (g <= 4) */
        for (int d = 1; d * d <= g; ++d) {
            if (g % d != 0) continue;
            /* first divisor d */
            if (d == g / d) {
                int mu = mobius_small(d);
                if (mu != 0) {
                    int nd = (int)(n / d);
                    int kd = k / d;           /* integer division */
                    long long bin = small_binom(nd, kd);
                    total += (long long)mu * bin;
                }
            } else {
                int d1 = d, d2 = g / d;
                int mu1 = mobius_small(d1);
                if (mu1 != 0) {
                    int nd = (int)(n / d1);
                    int kd = k / d1;
                    long long bin = small_binom(nd, kd);
                    total += (long long)mu1 * bin;
                }
                int mu2 = mobius_small(d2);
                if (mu2 != 0) {
                    int nd = (int)(n / d2);
                    int kd = k / d2;
                    long long bin = small_binom(nd, kd);
                    total += (long long)mu2 * bin;
                }
            }
        }
    }

    /* divide by n */
    seq_t result = (seq_t)(total / (long long)n);
    return result;
}
