/*
 * A386784  a(n) = n^4*sigma_4(n).
 * https://oeis.org/A386784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^4*sigma_4(n) (sigma_4 is the sum of fourth powers of
 *            divisors)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> sigma_vec;
static idx_t cache_size = 0;
static void ensure_cache(idx_t max_n) {
    size_t new_size = (size_t)(max_n + 1);
    if (sigma_vec.size() < new_size)
        sigma_vec.resize(new_size, (seq_t)0);
    else
        for (size_t i = 0; i <= max_n; ++i)
            sigma_vec[i] = (seq_t)0;
    cache_size = new_size;

    for (idx_t i = 1; i <= max_n; ++i) {
        seq_t pow4 = (seq_t)i * (seq_t)i * (seq_t)i * (seq_t)i;
        for (idx_t j = i; j <= max_n; j += i)
            sigma_vec[j] += pow4;
    }
}
#else
static seq_t *sigma = NULL;
static idx_t cache_size = 0;
static void ensure_cache(idx_t max_n) {
    size_t new_size = (size_t)(max_n + 1);
    seq_t *new_arr = realloc(sigma, new_size * sizeof(seq_t));
    if (!new_arr) abort();
    sigma = new_arr;
    for (size_t i = 0; i < new_size; ++i)
        sigma[i] = (seq_t)0;
    cache_size = new_size;

    for (idx_t i = 1; i <= max_n; ++i) {
        seq_t pow4 = (seq_t)i * (seq_t)i * (seq_t)i * (seq_t)i;
        for (idx_t j = i; j <= max_n; j += i)
            sigma[j] += pow4;
    }
}
#endif

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    ensure_cache(n);

#ifdef __cplusplus
    seq_t *sig = sigma_vec.data();
#else
    seq_t *sig = sigma;
#endif

    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t n4 = n_sq * n_sq;          // n^4
    return n4 * sig[(size_t)n];
}
