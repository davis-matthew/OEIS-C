/*
 * A373051  Number of non-similar triangles possible with distinct positive
 *          integer side lengths of at most n units.
 * https://oeis.org/A373051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d=1}^{n} μ(d) * T(⌊n/d⌋) where T(m) = number of
 *            integer triangles (x>y>z≥1, z+y>x) with distinct sides ≤ m
 * Range:     u8 n <= 16, u16 n <= 99, u32 n <= 3664, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Möbius function cache ---------- */
static int8_t *mu = NULL;           /* μ[0] unused, μ[1] = 1  */
static size_t mu_size = 0;          /* allocated size of mu array */

/* Compute μ(k) for all 1 ≤ k ≤ N using a linear sieve.  */
static void compute_mu_upto(idx_t N)
{
    /* Ensure the array is large enough.  */
    if (mu == NULL || mu_size <= (size_t)N) {
        size_t new_size = (size_t)N + 1;
        int8_t *tmp = (int8_t *)realloc(mu, new_size * sizeof(int8_t));
        if (!tmp) exit(1);
        mu = tmp;
        mu_size = new_size;
    }

    /* Allocate temporary arrays for the sieve.  */
    bool *is_comp = (bool *)calloc((size_t)N + 1, sizeof(bool));
    if (!is_comp) exit(1);
    idx_t *primes = (idx_t *)malloc(128 * sizeof(idx_t));
    if (!primes) { free(is_comp); exit(1); }
    size_t primes_cnt = 0, primes_cap = 128;

    mu[0] = 0;
    if (N >= 1) mu[1] = 1;

    for (idx_t i = 2; i <= N; ++i) {
        if (!is_comp[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                idx_t *tmp2 = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
                if (!tmp2) { free(is_comp); free(primes); exit(1); }
                primes = tmp2;
            }
            primes[primes_cnt++] = i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            idx_t p = primes[j];
            idx_t ip = i * p;
            if (ip > N) break;
            is_comp[ip] = true;
            if (i % p == 0) {
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = -mu[i];
            }
        }
    }

    free(is_comp);
    free(primes);
}

/* ---------- Count of all integer triangles with distinct sides ≤ n ---------- */
static inline seq_t count_triangles(idx_t n)
{
    if (n < 3) return (seq_t)0;

    idx_t M = n / 2;
    idx_t n_odd = (n - 1) / 2;

    seq_t sum_even = 0, sum_odd = 0;

    if (M >= 2) {
        seq_t m1 = (seq_t)(M - 1);
        seq_t m2 = (seq_t)M;
        seq_t m3 = (seq_t)(2 * M - 1);
        sum_even = m1 * m2 * m3 / (seq_t)6;
    }

    if (n_odd > 0) {
        seq_t no  = (seq_t)n_odd;
        seq_t no1 = no + (seq_t)1;
        seq_t no2 = (seq_t)(2 * n_odd + 1);
        seq_t sum_no = no * no1 * no2 / (seq_t)6;      /* Σ k^2   (k=1..n_odd) */
        seq_t sum_n  = no * no1 / (seq_t)2;            /* Σ k     (k=1..n_odd) */
        sum_odd = sum_no - sum_n;                     /* Σ k*(k-1) */
    }

    return sum_even + sum_odd;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    compute_mu_upto(n);

    seq_t res = (seq_t)0;
    for (idx_t d = 1; d <= n; ++d) {
        int8_t mu_d = mu[d];
        if (mu_d == 0) continue;
        idx_t k = n / d;
        seq_t t = count_triangles(k);
        if (mu_d == 1) {
            res += t;
        } else {                           /* mu_d == -1 */
            res -= t;
        }
    }
    return res;
}
