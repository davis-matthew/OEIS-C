/*
 * A334211  a(n) = 1 + a(n-1)*(A007018(n-1) + 1), with a(0) = 1.
 * https://oeis.org/A334211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + a(n-1)*(A007018(n-1) + 1) a(0) = 1 Recurrence for
 *            A007018: A007018(0)=1; A007018(n) =
 *            A007018(n-1)*(A007018(n-1)+1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* cache of a(n) */
static seq_t *b_cache = NULL;   /* cache of A007018(n) */
static size_t cached_cap = 0;   /* allocated capacity */
static size_t cached_len = 0;   /* number of elements actually computed (<= cached_cap) */

static void ensure_cache_size(size_t n) {
    if (n + 1 <= cached_cap)
        return;
    size_t new_cap = cached_cap ? cached_cap : 16;
    while (new_cap <= n)
        new_cap <<= 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    b_cache = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
    cached_cap = new_cap;
}

seq_t a(idx_t n) {
    size_t N = (size_t)n;
    ensure_cache_size(N);
    for (size_t i = cached_len; i <= N; ++i) {
        if (i == 0) {
            a_cache[0] = (seq_t)1;
            b_cache[0] = (seq_t)1;
        } else {
            seq_t prev_b = b_cache[i - 1];
            seq_t v = prev_b + (seq_t)1;      /* v = A007018(i-1) + 1 */
            b_cache[i] = prev_b * v;          /* A007018(i)   = prev_b * (prev_b + 1) */
            a_cache[i] = (seq_t)1 + a_cache[i - 1] * v; /* a(i) */
        }
    }
    cached_len = N + 1;
    return a_cache[N];
}
