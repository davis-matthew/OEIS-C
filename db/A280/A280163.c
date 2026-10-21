/*
 * A280163  Least number k such that sopfr(k) - sopf(k) = k/n, -1 if such a
 *          number does not exist.
 * https://oeis.org/A280163
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 58, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Formula: a(n) = least k such that sopfr(k) – sopf(k) = k/n, -1 if no such k exists.
   Offset: 2 (so a(2) is the first term). */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

#define MAX_CACHED_M 40000   /* accommodates 2*n for n up to 20000 (well beyond 10000) */

static uint64_t diff_cache[MAX_CACHED_M + 1];
static bool cached = false;
static void init_cache(void) {
    if (cached) return;
    cached = true;
    diff_cache[0] = 0;
    diff_cache[1] = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    for (idx_t m = 2; m <= MAX_CACHED_M; ++m) {
        factor_u64(m, primes, exps, &cnt);
        uint64_t d = 0;
        for (size_t i = 0; i < cnt; ++i)
            d += (exps[i] - 1) * primes[i];
        diff_cache[m] = d;
    }
}

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)-1;          /* below offset */
    init_cache();

    /* prime case: answer is n^2 */
    if (is_prime(n)) {
        uint64_t sq = (uint64_t)n * (uint64_t)n;
        return (seq_t)sq;
    }

    /* factor n once */
    uint64_t primes_n[64];
    uint64_t exps_n[64];
    size_t cnt_n;
    factor_u64(n, primes_n, exps_n, &cnt_n);

    uint64_t diff_n = 0;
    for (size_t i = 0; i < cnt_n; ++i)
        diff_n += (exps_n[i] - 1) * primes_n[i];

    idx_t limit = 2 * n;
    if (limit > MAX_CACHED_M) limit = MAX_CACHED_M;   /* safety */

    for (idx_t m = 1; m <= limit; ++m) {
        uint64_t diff_m = diff_cache[m];

        /* sum of distinct primes of n that divide m */
        uint64_t sum_common = 0;
        for (size_t i = 0; i < cnt_n; ++i)
            if (m % primes_n[i] == 0)
                sum_common += primes_n[i];

        uint64_t diff_nm = diff_n + diff_m + sum_common;
        if (diff_nm == (uint64_t)m) {
            uint64_t k = (uint64_t)n * (uint64_t)m;
            return (seq_t)k;
        }
    }
    return (seq_t)-1;   /* no solution found within bound */
}
