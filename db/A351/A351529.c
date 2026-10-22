/*
 * A351529  The number of quaternary strings of length n containing 00.
 * https://oeis.org/A351529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - b(n) where b(0)=1, b(1)=4, b(n)=3*b(n-1)+3*b(n-2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *b_cache = NULL;
static size_t b_cap = 0;
static size_t b_len = 0;

static seq_t *pow4_cache = NULL;
static size_t pow4_cap = 0;
static size_t pow4_len = 0;

static void ensure_b(idx_t n)
{
    if (b_cap <= n) {
        size_t new_cap = b_cap ? b_cap << 1 : 8;
        while (new_cap <= n) new_cap <<= 1;
        b_cache = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
        b_cap = new_cap;
    }
    if (b_len == 0) {
        b_cache[0] = 1;
        if (n >= 1) b_cache[1] = 4;
        b_len = 2;
    }
    for (idx_t i = b_len; i <= n; ++i) {
        seq_t t1 = b_cache[i-1];
        seq_t t2 = b_cache[i-2];
        b_cache[i] = 3 * t1 + 3 * t2;
    }
    if (b_len <= n) b_len = n + 1;
}

static void ensure_pow4(idx_t n)
{
    if (pow4_cap <= n) {
        size_t new_cap = pow4_cap ? pow4_cap << 1 : 8;
        while (new_cap <= n) new_cap <<= 1;
        pow4_cache = (seq_t *)realloc(pow4_cache, new_cap * sizeof(seq_t));
        pow4_cap = new_cap;
    }
    if (pow4_len == 0) {
        pow4_cache[0] = 1;
        if (n >= 1) pow4_cache[1] = 4;
        pow4_len = 2;
    }
    for (idx_t i = pow4_len; i <= n; ++i) {
        pow4_cache[i] = pow4_cache[i-1] * 4;
    }
    if (pow4_len <= n) pow4_len = n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_b(n);
    ensure_pow4(n);
    return pow4_cache[n] - b_cache[n];
}
