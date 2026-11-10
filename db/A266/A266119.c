/*
 * A266119  First differences of A265899.
 * https://oeis.org/A266119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A265899(n+1) - A265899(n); A265899 computed by scanning
 *            msd of (2m)!/m! in factorial base via logs
 * Range:     u8 n <= 1502, u16 n <= 1502, u32 n <= 1502, u64 n <= 1502,
 *            u128 n <= 1502, big any n
 * Validated: standard, pass, 136 terms through n = 1498 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/mem.h"

#define A266119_MAX_LOG 200000

static double *A266119_log_fact = NULL;
static bool A266119_init_done = false;

static void A266119_init_once(void)
{
    if (A266119_init_done) return;
    A266119_init_done = true;
    A266119_log_fact = (double*)malloc((A266119_MAX_LOG + 1) * sizeof(double));
    A266119_log_fact[0] = 0.0;
    for (size_t i = 1; i <= A266119_MAX_LOG; ++i)
        A266119_log_fact[i] = A266119_log_fact[i - 1] + log((double)i);
}

static size_t A266119_cache_len = 0;
static size_t A266119_cache_cap = 0;
static seq_t *A266119_cache_ptr = NULL;

static void A266119_ensure_cap(size_t needed)
{
    if (needed <= A266119_cache_cap) return;
    size_t new_cap = A266119_cache_cap ? A266119_cache_cap * 2 : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, A266119_cache_ptr, new_cap);
    if (!tmp) exit(1);
    A266119_cache_ptr = tmp;
    A266119_cache_cap = new_cap;
}

static seq_t A265899_a(idx_t N)
{
    if (N == 0) return 0;
    A266119_init_once();

    static idx_t last_processed = 0;
    static double log_m = 0.0;
    static idx_t k = 0;
    static idx_t prev_ms = 0;

    while (A266119_cache_len < (size_t)N) {
        idx_t n = ++last_processed;

        if (n == 1) {
            log_m = log(2.0);
        } else {
            log_m += log(2.0 * (double)n) + log(2.0 * (double)n - 1.0) - log((double)n);
        }

        while (k + 1 <= 2 * n && A266119_log_fact[k + 1] <= log_m + 1e-12)
            ++k;

        idx_t lo = 1, hi = k, msd = 1;
        while (lo <= hi) {
            idx_t mid = lo + (hi - lo) / 2;
            double test = A266119_log_fact[k] + log((double)mid);
            if (test <= log_m + 1e-12) {
                msd = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }

        if (n == 1 || msd <= prev_ms) {
            A266119_ensure_cap(A266119_cache_len + 1);
            A266119_cache_ptr[A266119_cache_len++] = (seq_t)n;
        }
        prev_ms = msd;
    }

    return A266119_cache_ptr[N - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t hi = A265899_a(n + 1);
    seq_t lo = A265899_a(n);
    return hi - lo;
}
