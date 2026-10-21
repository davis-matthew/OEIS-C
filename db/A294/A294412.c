/*
 * A294412  Solution of the complementary equation a(n) = a(n-1) + 2*b(n-2) +
 *          1, where a(0) = 1, a(1) = 3, b(0) = 2, b(1) = 4.
 * https://oeis.org/A294412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*b(n-2) + 1, a(0) = 1, a(1) = 3, b(0) = 2, b(1)
 *            = 4
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is memoised lazily; it works for all defined builds
   (uintN_t or arbitrary‑precision seq_t) and for all idx_t values.  */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* static storage for the two complementary sequences */
static seq_t *a_cache = NULL;
static seq_t *b_cache = NULL;
static idx_t cache_len = 0;   /* next free index + 1 */

/* bitset marking which integers are already used in a or b */
static uint64_t *used_bits = NULL;
static size_t used_bits_len = 0;   /* number of uint64_t words allocated */

/* mark v as used, enlarging the bitset if necessary */
static void set_used(idx_t v)
{
    size_t w = v >> 6;
    size_t b = v & 63;
    if (w >= used_bits_len) {
        size_t new_len = used_bits_len ? used_bits_len * 2 : 1;
        while (new_len <= w) new_len <<= 1;
        uint64_t *new_bits = (uint64_t *)realloc(used_bits, new_len * sizeof(uint64_t));
        if (!new_bits) exit(1);
        if (new_len > used_bits_len)
            memset(new_bits + used_bits_len, 0, (new_len - used_bits_len) * sizeof(uint64_t));
        used_bits = new_bits;
        used_bits_len = new_len;
    }
    used_bits[w] |= (uint64_t)1 << b;
}

/* test if v has already been used */
static int used_test(idx_t v)
{
    size_t w = v >> 6;
    if (w >= used_bits_len) return 0;
    return (used_bits[w] >> (v & 63)) & 1;
}

/* initialise the first two terms of a and b and mark them as used */
static void init_sequences(void)
{
    a_cache = (seq_t *)calloc(2, sizeof(seq_t));
    b_cache = (seq_t *)calloc(2, sizeof(seq_t));
    if (!a_cache || !b_cache) exit(1);
    a_cache[0] = (seq_t)1;
    a_cache[1] = (seq_t)3;
    b_cache[0] = (seq_t)2;
    b_cache[1] = (seq_t)4;
    cache_len = 2;
    set_used((idx_t)a_cache[0]);
    set_used((idx_t)a_cache[1]);
    set_used((idx_t)b_cache[0]);
    set_used((idx_t)b_cache[1]);
}

/* extend the cached sequences up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;
    size_t new_len = n + 1;
    seq_t *na = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
    seq_t *nb = (seq_t *)realloc(b_cache, new_len * sizeof(seq_t));
    if (!na || !nb) exit(1);
    a_cache = na;
    b_cache = nb;
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t ai = a_cache[i-1] + (seq_t)2 * b_cache[i-2] + (seq_t)1;
        a_cache[i] = ai;
        set_used((idx_t)ai);
        seq_t cand = b_cache[i-1] + (seq_t)1;
        while (used_test((idx_t)cand)) ++cand;
        b_cache[i] = cand;
        set_used((idx_t)cand);
    }
    cache_len = new_len;
}

/* required public function */
seq_t a(idx_t n)
{
    if (!a_cache) init_sequences();
    ensure_cache(n);
    return a_cache[n];
}
