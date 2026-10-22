/*
 * A321877  Square array A(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of Product_{j>=1} (1 + x^j)^sigma_k(j).
 * https://oeis.org/A321877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read from the square array A(n,k) defined by * A(n,k) =
 *            [x^n] ∏_{j≥1}(1+x^j)^{σ_k(j)}, * where σ_k(m) is the sum of the
 *            k‑th powers of the divisors of m. * The sequence a(n) consists
 *            of the entries of this array read by * antidiagonals: for each s
 *            = 0,1,2,… output A(0,s), A(1,s‑1), … , A(...
 * Range:     u8 n <= 38, u16 n <= 70, u32 n <= 142, u64 n <= 284,
 *            u128 n <= 541, big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_k.h"

#define MAX_T 200  /* sufficient for n up to a few million */

/* memoised values of A(t,k) : t (row), k (column)*/
static seq_t cache[MAX_T + 1][MAX_T + 1];
static bool  computed[MAX_T + 1][MAX_T + 1];

/* Forward declaration – required for static helper below */
static seq_t compute_A_entry(idx_t t, idx_t k);

/* Public interface */
seq_t a(idx_t n) {
    /* find the antidiagonal s such that (s+1)(s+2)/2 > n */
    idx_t s = 0;
    /* while ((s+1)*(s+2)/2 <= n) ++s;  // safe because s small */
    for (s = 0; (s + 1) * (s + 2) / 2 <= n; ++s) {
        /* nothing */
    }
    /* t is the offset on this diagonal */
    idx_t prevC = s * (s + 1) / 2;     /* number of terms before diagonal s */
    idx_t t = n - prevC;               /* 0‑based offset */
    if (t == 0) {                      /* first element of every diagonal */
        return 1;
    }
    idx_t k = s - t;                    /* corresponding column index */
    return compute_A_entry(t, k);
}

/* Helper: compute A(t,k) using a bounded‑knapsack DP */
static seq_t compute_A_entry(idx_t t, idx_t k) {
    if (t > MAX_T || k > MAX_T) return 0;   /* safety */
    if (computed[t][k]) return cache[t][k];

    /* DP arrays */
    seq_t dp[MAX_T + 1]      = {0};
    seq_t newdp[MAX_T + 1]   = {0};

    dp[0] = 1;   /* base case: empty partition gives exponent 0 */

    for (idx_t j = 1; j <= t; ++j) {
        /* multiplicity bound for part j */
        seq_t a = sigma_k(j, (uint8_t)k);

        idx_t mmax = t / j;  /* maximum copies of j that fit into sum t */

        /* zero the next dp */
        for (idx_t i = 0; i <= t; ++i) {
            newdp[i] = 0;
        }

        for (idx_t sprev = 0; sprev <= t; ++sprev) {
            seq_t val = dp[sprev];
            if (val == 0) continue;

            seq_t binom = 1;          /* C(a,0) */
            idx_t m = 0;
            while (1) {
                idx_t ns = sprev + m * j;
                if (ns > t) break;
                /* add contribution */
                newdp[ns] += val * binom;

                if (m == mmax) break;
                /* compute binom = binom * (a - m) / (m+1) */
                seq_t term = a - m;
                binom = binom * term / (m + 1);
                ++m;
            }
        }

        /* copy newdp to dp for next iteration */
        for (idx_t i = 0; i <= t; ++i) {
            dp[i] = newdp[i];
        }
    }

    computed[t][k] = true;
    cache[t][k] = dp[t];
    return dp[t];
}
