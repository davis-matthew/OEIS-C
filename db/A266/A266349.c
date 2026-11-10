/*
 * A266349  a(n) = 1 + A053644(n) - A004001(n+1) = 1 + A072376(n) -
 *          A266348(n).
 * https://oeis.org/A266349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + A053644(n) - A004001(n+1), where A053644(n) =
 *            2^floor(log2(n)) and A004001(m) = A004001(A004001(m-1)) +
 *            A004001(m-A004001(m-1)), A004001(1) = A004001(2) = 1
 * Range:     u8 n <= 511, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 112 terms through n = 7525 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A004001_cache = NULL;
static idx_t A004001_len = 0; /* valid indices 1..A004001_len-1 */

static seq_t A004001_a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n >= A004001_len) {
        idx_t new_len = n + 1;
        if (new_len < 3) new_len = 3;
        if (new_len < 2 * A004001_len) new_len = 2 * A004001_len;
        seq_t *nc = mem_new(seq_t, new_len);
        idx_t i;
        for (i = 0; i < A004001_len; i++) nc[i] = A004001_cache[i];
        if (A004001_cache) mem_free(A004001_cache);
        A004001_cache = nc;
        if (A004001_len < 3) {
            A004001_cache[1] = (seq_t)1;
            A004001_cache[2] = (seq_t)1;
            A004001_len = 3;
        }
        for (i = A004001_len; i < new_len; i++) {
            idx_t p = (idx_t)A004001_cache[i - 1];
            A004001_cache[i] = A004001_cache[p] + A004001_cache[i - p];
        }
        A004001_len = new_len;
    }
    return A004001_cache[n];
}

static seq_t A053644_a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t r = 1;
    while (r <= (n >> 1)) r <<= 1;
    return (seq_t)r;
}

seq_t a(idx_t n)
{
    return (seq_t)1 + A053644_a(n) - A004001_a(n + 1);
}
