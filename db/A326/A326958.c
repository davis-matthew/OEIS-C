/*
 * A326958  Total sum of noncomposite parts in all partitions of n.
 * https://oeis.org/A326958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 28, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{k∈{1}∪Primes, k≤n} k * Σ_{i≥1} i * p(n − i·k)
 * where p(m) is the partition number of m.  The implementation
 * computes all partition numbers up to the requested index and
 * then evaluates the above double sum using an incremental
 * recurrence for each non‑composite part k.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static seq_t *p_arr   = NULL;   /* partition numbers p[0..max]   */
static seq_t *a_arr   = NULL;   /* desired sequence a[0..max]   */
static bool *prime_arr = NULL;  /* sieve of Eratosthenes       */
static idx_t cached_max = 0;    /* maximum n for which data is ready */

/* Recompute all tables up to N (inclusive).  N must be non‑negative. */
static void recompute_up_to(idx_t N)
{
    size_t cap = (size_t)N + 1;

    /* Allocate or resize arrays. */
    p_arr   = (seq_t *)realloc(p_arr,   cap * sizeof(seq_t));
    a_arr   = (seq_t *)realloc(a_arr,   cap * sizeof(seq_t));
    prime_arr = (bool *)realloc(prime_arr, cap * sizeof(bool));

    /* Initialize to zero. */
    memset(p_arr, 0, cap * sizeof(seq_t));
    memset(a_arr, 0, cap * sizeof(seq_t));
    memset(prime_arr, 0, cap * sizeof(bool));

    /* ----- Sieve of Eratosthenes ----- */
    if (N >= 2) {
        for (size_t i = 2; i <= N; ++i) prime_arr[i] = true;
        for (idx_t p = 2; p * p <= N; ++p) {
            if (prime_arr[p]) {
                for (idx_t m = p * p; m <= N; m += p)
                    prime_arr[m] = false;
            }
        }
    }

    /* ----- Partition numbers: classical DP (addition only) ----- */
    p_arr[0] = (seq_t)1;
    for (idx_t i = 1; i <= N; ++i) {
        for (idx_t j = i; j <= N; ++j) {
            p_arr[j] += p_arr[j - i];
        }
    }

    /* ----- Compute a(n) using the recurrence for each non‑composite part ----- */
    seq_t *cnt = (seq_t *)malloc(cap * sizeof(seq_t));

    for (idx_t k = 1; k <= N; ++k) {
        if (k == 1 || prime_arr[k]) {
            /* Reset temporary array */
            memset(cnt, 0, cap * sizeof(seq_t));

            /* Compute count_k(n) for all n ≥ k */
            for (idx_t n = k; n <= N; ++n) {
                cnt[n] = cnt[n - k] + p_arr[n - k];
            }

            /* Accumulate contribution of part k to a[n] */
            for (idx_t n = k; n <= N; ++n) {
                a_arr[n] += (seq_t)k * cnt[n];
            }
        }
    }

    free(cnt);
    cached_max = N;
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    if (n > cached_max)
        recompute_up_to(n);

    return a_arr[(size_t)n];
}
