/*
 * A276130  a(0) = a(1) = a(2) = a(3) = a(4) = 1; for n>4, a(n) = ( a(n-1)
 *          +a(n-3) )*( a(n-2)+a(n-4) ) / a(n-5).
 * https://oeis.org/A276130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 10, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A276130
   a(0) = a(1) = a(2) = a(3) = a(4) = 1;
   a(n) = (a(n-1)+a(n-3)) * (a(n-2)+a(n-4)) / a(n-5)   for n>4
*/
#include <stdlib.h>
#include "../utils/seq_mul.h"

static seq_t *seq_cache = NULL;
static size_t seq_cap   = 0;
static size_t seq_len   = 0;

static void seq_init(void)
{
    if (seq_cache != NULL)
        return;
    seq_cap = 16;
    seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (!seq_cache)
        exit(1);
    for (size_t i = 0; i < 5; ++i)
        seq_cache[i] = (seq_t)1;
    seq_len = 5;
}

static void seq_grow_to(size_t need)
{
    if (need < seq_cap)
        return;
    size_t new_cap = seq_cap ? (seq_cap << 1) : 16;
    if (new_cap <= need)
        new_cap = need + 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        free(seq_cache);
        seq_cache = NULL;
        exit(1);
    }
    seq_cache = tmp;
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    seq_init();

    size_t idx = (size_t)n;
    if (idx < seq_len)
        return seq_cache[idx];

    seq_grow_to(idx);
    for (size_t k = seq_len; k <= idx; ++k) {
        if (k < 5) {
            seq_cache[k] = (seq_t)1;
            continue;
        }
        seq_t sum1 = seq_cache[k-1] + seq_cache[k-3];
        seq_t sum2 = seq_cache[k-2] + seq_cache[k-4];
        seq_t prod = seq_mul(sum1, sum2);
        seq_cache[k] = prod / seq_cache[k-5];
    }
    seq_len = idx + 1;
    return seq_cache[idx];
}
