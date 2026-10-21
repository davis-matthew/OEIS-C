/*
 * A272727  a(0)=0; thereafter a(n+1) is the number of coincidences between
 *          the sequence so far (a(0), ..., a(n)) and its reverse (a(n), ...,
 *          a(0)).
 * https://oeis.org/A272727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of coincidences between sequence a(0..n-1) and its
 *            reverse
 * Range:     u8 n <= 510, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#if defined(__cplusplus)
#include <vector>
#endif

#if defined(__cplusplus)
static std::vector<seq_t> seq_cache;
static size_t cache_len = 0;
#else
static seq_t *seq_cache = NULL;
static size_t cache_len = 0;
#endif

seq_t a(idx_t n)
{
#if defined(__cplusplus)
    /* Initialize cache for the first call. */
    if (seq_cache.empty()) {
        seq_cache.reserve(1);
        seq_cache.push_back((seq_t)0); /* a(0) = 0 */
        cache_len = 1;
    }

    /* Extend cache up to index n if needed. */
    if (cache_len <= (size_t)n) {
        seq_cache.resize(n + 1);
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t cnt = 0;
            for (size_t j = 0; j < i; ++j) {
                if (seq_cache[j] == seq_cache[i - 1 - j]) {
                    ++cnt;
                }
            }
            seq_cache[i] = cnt;
        }
        cache_len = seq_cache.size();
    }
    return seq_cache[(size_t)n];
#else
    /* Initialize cache for the first call. */
    if (cache_len == 0) {
        seq_cache = calloc(1, sizeof(seq_t));
        if (!seq_cache) return (seq_t)0;          /* unlikely for test cases */
        seq_cache[0] = (seq_t)0;                 /* a(0) = 0 */
        cache_len = 1;
    }

    /* Extend cache up to index n if needed. */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = realloc(seq_cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;               /* unlikely for test cases */
        seq_cache = tmp;
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t cnt = 0;
            for (size_t j = 0; j < i; ++j) {
                if (seq_cache[j] == seq_cache[i - 1 - j]) {
                    ++cnt;
                }
            }
            seq_cache[i] = cnt;
        }
        cache_len = new_len;
    }
    return seq_cache[(size_t)n];
#endif
}
