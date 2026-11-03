/*
 * A331780  One-half of the number of lines through at least 2 points of an n
 *          X n grid of points.
 * https://oeis.org/A331780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A331780 - One-half of the number of lines through at least 2 points of an n×n grid.
 *  The algorithm counts all distinct grid lines of slope 0, ∞, and every
 *  primitive direction (dx,dy) with 1≤dx,dy≤n-1.  It uses Möbius inversion
 *  to sum over coprime pairs efficiently.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: enumerate all divisors of n (n≥1) using spf array. */
static void divisors_of(int n, const uint16_t *spf, int *divs, int *cnt) {
    int orig = n;
    int primes[20], exps[20], pc = 0;
    while (n > 1) {
        int p = spf[n];
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        primes[pc] = p;
        exps[pc++] = e;
    }
    /* recursively generate divisors */
    *cnt = 1;
    divs[0] = 1;
    for (int i = 0; i < pc; ++i) {
        int p = primes[i];
        int e = exps[i];
        int curCnt = *cnt;
        int mult = 1;
        for (int k = 1; k <= e; ++k) {
            mult *= p;
            for (int j = 0; j < curCnt; ++j) {
                divs[*cnt] = divs[j] * mult;
                ++(*cnt);
            }
        }
    }
}

/* Helper: compute all smallest prime factors and Möbius function up to M. */
static void sieve_spf_mu(int M, uint16_t *spf, int8_t *mu) {
    int i, j;
    int *primes = (int*)malloc((M + 1) * sizeof(int));
    int pc = 0;
    for (i = 0; i <= M; ++i) spf[i] = 0;
    mu[0] = 0; mu[1] = 1;
    for (i = 2; i <= M; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            mu[i] = -1;
            primes[pc++] = i;
        }
        for (j = 0; j < pc && primes[j] <= spf[i] && (long long)i * primes[j] <= M; ++j) {
            int p = primes[j];
            spf[i * p] = p;
            if (p == spf[i]) {
                mu[i * p] = 0;
            } else {
                mu[i * p] = -mu[i];
            }
        }
    }
    free(primes);
}

/* Main function implementing the sequence */
seq_t a(idx_t n) {
    if (n <= 1)
        return 0;

    idx_t N = n;                 /* duplicate to avoid mix with seq_t */
    int M = (int)(N - 1);        /* maximum coordinate value */

    /* Allocate and compute auxiliary arrays */
    uint16_t *spf = (uint16_t*)malloc((M + 1) * sizeof(uint16_t));
    int8_t *mu   = (int8_t*)  malloc((M + 1) * sizeof(int8_t));
    sieve_spf_mu(M, spf, mu);

    /* Precompute sums over multiples for functions (n - d) and g(d) = max(0,n-2d) */
    uint64_t *S_mul_n = (uint64_t*)malloc((M + 1) * sizeof(uint64_t));
    uint64_t *S_mul_g = (uint64_t*)malloc((M + 1) * sizeof(uint64_t));

    int lim_g = (int)((N - 1) / 2);     /* largest d with n - 2d > 0 */

    for (int d = 1; d <= M; ++d) {
        int T = M / d;
        /* sum_{t=1..T} (N - d*t) */
        uint64_t sum_n = (uint64_t)N * T - (uint64_t)d * T * (T + 1) / 2;
        S_mul_n[d] = sum_n;

        int T2 = lim_g / d;
        if (T2 > T) T2 = T;
        uint64_t sum_g = (uint64_t)N * T2 - (uint64_t)2 * d * T2 * (T2 + 1) / 2;
        S_mul_g[d] = sum_g;
    }

    /* Count horizontal and vertical lines (each n lines, provided n ≥ 2) */
    seq_t total_lines = (seq_t)N * 2;   /* 2*n */

    /* Temporary arrays for divisors */
    int divs[1024];
    int dcnt;

    /* Iterate over all dx ≥ 1 */
    for (int dx = 1; dx <= M; ++dx) {
        /* compute sums over dy coprime with dx using Möbius inversion */
        divisors_of(dx, spf, divs, &dcnt);

        int64_t sum1 = 0;   /* Σ μ(d) * S_mul_n[d] */
        int64_t sum2 = 0;   /* Σ μ(d) * S_mul_g[d] */

        for (int i = 0; i < dcnt; ++i) {
            int d = divs[i];
            if (mu[d] == 0) continue;
            sum1 += (int64_t)mu[d] * (int64_t)S_mul_n[d];
            sum2 += (int64_t)mu[d] * (int64_t)S_mul_g[d];
        }

        seq_t nxdiv = (seq_t)(N - dx);
        seq_t gdx = (seq_t)((N > 2 * dx) ? (N - 2 * dx) : 0);

        seq_t contrib = nxdiv * (seq_t)sum1 - gdx * (seq_t)sum2;

        total_lines += contrib * 2;      /* multiply by 2 for dy>0 and dy<0 */
    }

    free(spf);
    free(mu);
    free(S_mul_n);
    free(S_mul_g);

    /* a(n) is half the total number of distinct lines. */
    return total_lines / 2;
}
