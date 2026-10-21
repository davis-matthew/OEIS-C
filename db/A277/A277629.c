/*
 * A277629  Number of aperiodic necklaces (Lyndon words) with k<=5 black beads
 *          and n-k white beads.
 * https://oeis.org/A277629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/n) * Σ_{k=0}^5 Σ_{d|gcd(n,k)} μ(d) * C(n/d, k/d)
 * Range:     u8 n <= 10, u16 n <= 25, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   mul_binom_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* μ(1)=1, μ(2)=-1, μ(3)=-1, μ(4)=0, μ(5)=-1 */
#include <stdint.h>
#include "../utils/mul_binom_small.h"

/* local signed gcd for idx_t */
static idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* small binomial for n <= 10000 computed once */
#define MAXBINOM 10000
static seq_t binom_tbl[MAXBINOM + 1][6];
static int binom_ready = 0;

/* compute binom_tbl up to MAXBINOM */
static void init_binom_tbl(void) {
    if (binom_ready) return;
    for (idx_t n = 0; n <= MAXBINOM; ++n) {
        binom_tbl[n][0] = 1;
        for (idx_t k = 1; k <= 5 && k <= n; ++k) {
            if (k == n)
                binom_tbl[n][k] = 1;
            else
                binom_tbl[n][k] = binom_tbl[n-1][k] + binom_tbl[n-1][k-1];
        }
        for (idx_t k = n+1; k <= 5; ++k)
            binom_tbl[n][k] = 0;
    }
    binom_ready = 1;
}

/* mu array indexed 0..5 */
static const int mu_arr[6] = {0, 1, -1, -1, 0, -1};

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */
    init_binom_tbl();                      /* ensure binomial table is ready */

    /* handle weight 0 separately */
    int64_t total = 0;
    if (n == 1) total += 1;                /* k = 0 gives one word for n=1 */
    /* k >= 1 */
    for (idx_t k = 1; k <= 5 && k <= n; ++k) {
        idx_t g = gcd_idx(n, k);           /* gcd(n,k) <= 5 */
        int64_t sum_k = 0;
        /* iterate divisors of g */
        for (idx_t d = 1; d * d <= g; ++d) {
            if (g % d == 0) {
                /* divisor d */
                int mu_d = mu_arr[d];
                if (mu_d != 0) {
                    idx_t n2 = n / d;
                    idx_t k2 = k / d;
                    seq_t c = (n2 <= MAXBINOM) ?
                              binom_tbl[n2][k2] : mul_binom_small(n2, k2);
                    sum_k += (int64_t)mu_d * (int64_t)c;
                }
                /* complementary divisor g/d if distinct */
                if (d != g / d) {
                    idx_t d2 = g / d;
                    int mu_d2 = mu_arr[d2];
                    if (mu_d2 != 0) {
                        idx_t n2 = n / d2;
                        idx_t k2 = k / d2;
                        seq_t c = (n2 <= MAXBINOM) ?
                                  binom_tbl[n2][k2] : mul_binom_small(n2, k2);
                        sum_k += (int64_t)mu_d2 * (int64_t)c;
                    }
                }
            }
        }
        total += sum_k;
    }

    /* result must be non‑negative and divisible by n */
    seq_t ans = (seq_t)(total / (int64_t)n);
    return ans;
}
