/*
 * A321306  The number of connected weighted cubic graphs with weight n on 6
 *          vertices.
 * https://oeis.org/A321306
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of connected weighted cubic graphs with weight n
 *            on 6 vertices
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 196, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed from the cycle index sums for the two cubic graphs on 6 vertices */

#include <stdbool.h>   /* for bool type */

/* Compute binomial coefficient C(n,k) for 0 ≤ k ≤ n and n up to a few thousand.
   n and k are idx_t, the result may require seq_t. */
static seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 6)
        return 0;   /* offset starts at 6 */

    /* ----- contribution from the K3,3 graph (group size 12) ----- */
    seq_t sum_k33 = 0;

    /* t1^6 */
    sum_k33 += binom_seq(n - 1, 5);

    /* 3 * t1^2 * t2^2 */
    seq_t term_12_22 = 0;
    if (n >= 4) {
        idx_t L = (n - 2) / 2;
        for (idx_t t = 2; t <= L; ++t) {
            idx_t s = n - 2 * t;          /* remaining sum for a+b */
            if (s < 2)
                continue;
            term_12_22 += (seq_t)(t - 1) * (seq_t)(s - 1);
        }
    }
    sum_k33 += 3 * term_12_22;

    /* 2 * t3^2 */
    if (n % 3 == 0) {
        idx_t m = n / 3;
        if (m >= 2)
            sum_k33 += 2 * (seq_t)(m - 1);
    }

    /* 4 * t2^3 */
    if (n % 2 == 0) {
        idx_t m = n / 2;
        if (m >= 3) {
            seq_t val = binom_seq(m - 1, 2);
            sum_k33 += 4 * val;
        }
    }

    /* 2 * t6 */
    if (n % 6 == 0)
        sum_k33 += 2;

    /* ----- contribution from the triangular prism (group size 72) ----- */
    seq_t sum_prism = 0;

    /* t1^6 */
    sum_prism += binom_seq(n - 1, 5);

    /* 6 * t1^4 * t2 */
    if (n >= 6) {
        idx_t max_e = (n - 4) / 2;
        for (idx_t e = 1; e <= max_e; ++e) {
            idx_t S = n - 2 * e;   /* sum for 4 singleton weights */
            if (S < 4)
                continue;
            sum_prism += 6 * binom_seq(S - 1, 3);
        }
    }

    /* 9 * t1^2 * t2^2 (same count as above) */
    sum_prism += 9 * term_12_22;

    /* 4 * t1^3 * t3 */
    if (n >= 6) {
        idx_t max_d = (n - 3) / 3;
        for (idx_t d = 1; d <= max_d; ++d) {
            idx_t S = n - 3 * d;   /* sum for 3 singleton weights */
            if (S < 3)
                continue;
            sum_prism += 4 * binom_seq(S - 1, 2);
        }
    }

    /* 12 * t1 * t2 * t3 */
    if (n >= 6) {
        idx_t max_c = (n - 3) / 3;
        for (idx_t c = 1; c <= max_c; ++c) {
            idx_t R = n - 3 * c;
            if (R < 3)
                continue;
            /* number of (a,b) with a + 2*b = R and a,b >=1 */
            idx_t cnt = (R - 1) / 2;
            sum_prism += 12 * (seq_t)cnt;
        }
    }

    /* 6 * t2^3 */
    if (n % 2 == 0) {
        idx_t m = n / 2;
        if (m >= 3) {
            seq_t val = binom_seq(m - 1, 2);
            sum_prism += 6 * val;
        }
    }

    /* 18 * t2 * t4 */
    if (n % 2 == 0) {
        idx_t max_b = (n - 2) / 4;
        for (idx_t b = 1; b <= max_b; ++b) {
            idx_t R = n - 4 * b;
            if (R < 2)
                continue;
            /* for each allowed b, there are 1 choices for a,b,c,d, so add 1*18 */
            sum_prism += 18;
        }
    }

    /* 12 * t6 */
    if (n % 6 == 0)
        sum_prism += 12;

    /* 4 * t3^2 */
    if (n % 3 == 0) {
        idx_t m = n / 3;
        if (m >= 2)
            sum_prism += 4 * (seq_t)(m - 1);
    }

    /* Final result: divide by group orders, exact rational divisions */
    seq_t part1 = sum_k33 / 12;   /* division is exact */
    seq_t part2 = sum_prism / 72; /* division is exact */
    return part1 + part2;
}
