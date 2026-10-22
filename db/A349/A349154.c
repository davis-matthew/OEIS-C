/*
 * A349154  Numbers k such that the k-th composition in standard order has sum
 *          equal to negative twice its alternating sum.
 * https://oeis.org/A349154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that the k-th composition in standard
 *            order has sum equal to negative twice its alternating sum;
 *            sequence of such k's (offset 1)
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_cnt = 0;
static size_t cache_cap = 0;
static idx_t tested_k = 0;

static bool qualifies(idx_t k)
{
    int positions[64];
    int m = 0;
    idx_t kk = k;
    int pos = 1;
    while (kk) {
        if (kk & 1ULL)
            positions[m++] = pos;
        kk >>= 1;
        pos++;
    }
    int64_t S = (m > 0) ? positions[m-1] : 0;        /* sum of parts = bit length */
    int64_t A = 0;                                   /* alternating sum */
    int sign = 1;
    for (int i = m - 1; i >= 0; --i) {
        int part = positions[i] - (i > 0 ? positions[i - 1] : 0);
        A += sign * part;
        sign = -sign;
    }
    return (S == (seq_t)(-2 * A));
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* safety for zero; actual offset is 1 */

    while (cache_cnt < n) {
        idx_t k = tested_k;
        if (qualifies(k)) {
            if (cache_cnt == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp)
                    exit(EXIT_FAILURE);
                cache = tmp;
            }
            cache[cache_cnt++] = (seq_t)k;
        }
        tested_k++;
    }
    return cache[n - 1];
}
