/*
 * A277397  Like 4-white numbers but with blocks of 4 starting at left.
 * https://oeis.org/A277397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   a(n) is the nth integer such that n⁴, when its decimal *
 *            representation is split into blocks of four digits starting from
 *            the * left, and those blocks are summed as integers, yields n. *
 *            Example: 14638⁴ = 45912080296849936 → 4591 + 2080 + 2968 + 4993
 *            + 6 = 14638
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, digit_len, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

#include "../utils/digit_len.h"   /* int digit_len(seq_t x); */
#include "../utils/seq_to_str.h"  /* void seq_to_str(seq_t v, char *buf, size_t *len); */
#include "../utils/big_pow.h"     /* seq_t big_pow(seq_t base, seq_t exp); */

static seq_t *cache      = NULL;   /* cached sequence values */
static size_t cache_len  = 0;      /* number of stored terms        */
static size_t cache_cap  = 0;      /* allocated capacity            */
static seq_t next_candidate = 0;   /* smallest integer not yet tested */

/* ensure cache has space for at least one more element */
static void ensure_cache_capacity(void)
{
    if (cache_len < cache_cap)
        return;
    size_t newcap = cache_cap == 0 ? 4 : cache_cap * 2;
    void *tmp = realloc(cache, newcap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = (seq_t *)tmp;
    cache_cap = newcap;
}

/* test if n is a “4‑white” number with 4‑digit blocks starting at the left */
static bool is_candidate(seq_t n)
{
    /* compute n^4 */
    seq_t pow4 = big_pow(n, (seq_t)4);

    /* convert to decimal string */
    size_t len = digit_len(pow4);
    /* allocate buffer of digits + one byte for null terminator */
    char *buf = (char *)malloc(len + 1);
    if (!buf)
        abort();

    seq_to_str(pow4, buf, &len);
    buf[len] = '\0';

    /* sum the 4‑digit blocks from the left */
    seq_t sum = 0;
    size_t i = 0;
    while (i < len) {
        size_t block_len = (len - i > 4) ? 4 : (len - i);
        seq_t block = 0;
        for (size_t j = 0; j < block_len; ++j) {
            block = block * 10 + (seq_t)(buf[i + j] - '0');
        }
        sum += block;
        i += block_len;
    }

    free(buf);
    return sum == n;
}

/* external‑linkage function required by the framework */
seq_t a(idx_t n)
{
    assert(n >= 1);   /* offset 1 */

    /* generate terms up to the requested index */
    while (cache_len < (size_t)n) {
        if (is_candidate(next_candidate)) {
            ensure_cache_capacity();
            cache[cache_len++] = next_candidate;
        }
        next_candidate = next_candidate + (seq_t)1;
    }
    return cache[(size_t)n - 1];
}
