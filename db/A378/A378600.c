/*
 * A378600  Signed variant of Zumkeller deficiency: a(n) = signum(A033879(n))
 *          * A103977(n).
 * https://oeis.org/A378600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = signum(A033879(n)) * A103977(n) A033879(n) = 2*n - σ(n)
 *            If A033879(n) ≥ 0, a(n) = A033879(n); otherwise a(n) =
 *            -A103977(n) (Zumkeller deficiency)
 * Range:     i8 n <= 130, i16 n <= 8737, i32 n <= 8737, i64 n <= 8737,
 *            i128 n <= 8737, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* sum of all divisors of n, including n itself */
static seq_t sigma_of(uint64_t n)
{
    seq_t sum = 1;                 /* initialise with divisor 1 */
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t term = 1;
            seq_t p_pow = 1;
            while (m % p == 0) {
                m /= p;
                p_pow *= (seq_t)p;   /* p^k */
                term += p_pow;       /* 1 + p + p^2 + … + p^k */
            }
            sum *= term;              /* multiply by factor contribution */
        }
    }
    if (m > 1) {                   /* remaining prime factor */
        sum *= ((seq_t)1 + (seq_t)m);
    }
    return sum;
}

/* Signed variant of Zumkeller deficiency: a(n) */
seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;

    seq_t sigma = sigma_of(N);
    seq_t deficiency = (seq_t)2 * (seq_t)N - sigma;

    if (deficiency >= 0) {
        return deficiency;          /* non‑abundant case */
    }

    /* Gather proper divisors (exclude N itself) */
    uint64_t divisors[256];
    size_t dcnt = 0;
    for (uint64_t d = 1; d * d <= N; ++d) {
        if (N % d == 0) {
            uint64_t d1 = d;
            uint64_t d2 = N / d;
            if (d1 != N) {
                divisors[dcnt++] = d1;
            }
            if (d2 != N && d2 != d1) {
                divisors[dcnt++] = d2;
            }
        }
    }

    /* Sum of proper divisors */
    uint64_t sum_total = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        sum_total += divisors[i];
    }
    int total = (int)sum_total;            /* used as array size (small n) */

    /* Subset sum DP to find minimal deficit to equal partition */
    uint8_t *dp = (uint8_t*)calloc(total + 1, sizeof(uint8_t));
    if (!dp) return 0; /* safety, though unlikely */
    dp[0] = 1;
    for (size_t idx = 0; idx < dcnt; ++idx) {
        int di = (int)divisors[idx];
        for (int s = total; s >= di; --s) {
            if (dp[s - di]) dp[s] = 1;
        }
    }

    /* Find largest achievable sum ≤ total/2 */
    int half = total / 2;
    seq_t best_diff = (seq_t)sum_total;    /* initialise with sum_total (worst) */
    for (int s = half; s >= 0; --s) {
        if (dp[s]) {
            best_diff = (seq_t)sum_total - (seq_t)(2 * (seq_t)s);
            break;
        }
    }
    free(dp);

    return -best_diff;                     /* negative of Zumkeller deficiency */
}
