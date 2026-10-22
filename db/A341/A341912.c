/*
 * A341912  Number of partitions of n into 5 distinct and relatively prime
 *          parts.
 * https://oeis.org/A341912
 *
 * Offset:    15    (a(15) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Formula: G.f.: Sum_{k>=1} mu(k)* x^(15*k) / Product_{j=1..5} (1 - x^(j*k)). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* static storage */
static bool init_done = false;
static idx_t max_n = 0;           /* largest n processed so far */
static int8_t *mu_arr = NULL;     /* Möbius function values 1..max_n */
static int64_t *ways_arr = NULL;  /* p(m)   for m = 0..max_n-15   */
static seq_t *res_arr = NULL;     /* a(n)  for n = 0..max_n */

/* linear sieve for Möbius function up to n */
static void compute_mobius(idx_t n)
{
    mu_arr = (int8_t*)realloc(mu_arr, (size_t)(n + 1) * sizeof(int8_t));
    if (!mu_arr) return;
    memset(mu_arr, 0, (size_t)(n + 1) * sizeof(int8_t));

    /* primes list */
    int *prime = (int*)malloc((size_t)(n + 1) * sizeof(int));
    if (!prime) return;
    int *is_comp = (int*)calloc((size_t)(n + 1), sizeof(int));
    if (!is_comp) { free(prime); return; }

    size_t pcnt = 0;
    mu_arr[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        if (!is_comp[i]) {
            prime[pcnt++] = (int)i;
            mu_arr[i] = -1;
        }
        for (size_t j = 0; j < pcnt && (idx_t)prime[j] * i <= n; ++j) {
            int p = prime[j];
            idx_t v = i * (idx_t)p;
            is_comp[v] = 1;
            if (i % (idx_t)p == 0) {
                mu_arr[v] = 0;
                break;
            } else {
                mu_arr[v] = -mu_arr[i];
            }
        }
    }
    free(prime);
    free(is_comp);
}

/* DP for partitions with parts 1..5 (unlimited) up to m = n-15 */
static void compute_ways(idx_t n)
{
    idx_t max_m = (n > 15 ? n - 15 : 0);
    ways_arr = (int64_t*)realloc(ways_arr, (size_t)(max_m + 1) * sizeof(int64_t));
    if (!ways_arr) return;
    memset(ways_arr, 0, (size_t)(max_m + 1) * sizeof(int64_t));
    ways_arr[0] = 1;
    for (int w = 1; w <= 5; ++w) {
        for (idx_t s = (idx_t)w; s <= max_m; ++s) {
            ways_arr[s] += ways_arr[s - (idx_t)w];
        }
    }
}

/* precompute all a(n) up to n */
static void precompute(idx_t n)
{
    if (init_done && n <= max_n) return;
    if (n < 15) n = 15;
    compute_mobius(n);
    compute_ways(n);
    res_arr = (seq_t*)realloc(res_arr, (size_t)(n + 1) * sizeof(seq_t));
    if (!res_arr) return;
    memset(res_arr, 0, (size_t)(n + 1) * sizeof(seq_t));
    for (idx_t i = 15; i <= n; ++i) {
        int64_t sum = 0;
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                idx_t d1 = d;
                idx_t d2 = i / d;
                if (i / d1 >= 15) {
                    sum += (int64_t)mu_arr[d1] *
                           ways_arr[(i / d1) - 15];
                }
                if (d1 != d2 && i / d2 >= 15) {
                    sum += (int64_t)mu_arr[d2] *
                           ways_arr[(i / d2) - 15];
                }
            }
        }
        res_arr[i] = (seq_t)sum;
    }
    max_n = n;
    init_done = true;
}

/* required function */
seq_t a(idx_t n)
{
    precompute(n);
    if (n < 15) return (seq_t)0;
    return res_arr[n];
}
