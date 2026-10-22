/*
 * A373941  Numbers whose base-2 representation is a "nested" palindrome.
 * https://oeis.org/A373941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th number whose binary representation is a nested
 *            palindrome
 * Range:     u8 n <= 13, u16 n <= 61, u32 n <= 1021, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t   cache_len = 0;
static size_t   cache_cap = 0;
static idx_t    next_len  = 1;  /* next binary length to generate (>=1) */

/* Reverse the lowest 'len' bits of x. */
static seq_t rev_bits(seq_t x, int len)
{
    seq_t r = 0;
    for (int i = 0; i < len; ++i) {
        r = (r << 1) | ((x >> i) & 1);
    }
    return r;
}

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 128;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Append a value to the cache. */
static void cache_append(seq_t val)
{
    if (cache_len >= cache_cap) ensure_cache_cap(cache_len + 1);
    cache[cache_len++] = val;
}

/* Generate all nested palindrome numbers of binary length 'len'
   and append them to the cache (in increasing numeric order). */
static void generate_length(idx_t len)
{
    /* The required half length for the left half of the number. */
    int L = (len + 1) >> 1;   /* ceil(len/2) */

    /* Palindromic H of length L (leading bit 1). */
    int half_len_H = (L + 1) >> 1;   /* ceil(L/2) */
    int free_H     = half_len_H - 1; /* bits that can be freely chosen */

    for (int mask = 0; mask < (1 << free_H); ++mask) {
        /* Construct the first half of H. */
        seq_t first_half = ((seq_t)1 << (half_len_H - 1)) | (seq_t)mask;

        /* Build the full palindrome H of length L. */
        seq_t H;
        if (L & 1) {
            int k = (L - 1) >> 1;
            H = (first_half << k) | rev_bits(first_half >> 1, k);
        } else {
            int k = L >> 1;
            H = (first_half << k) | rev_bits(first_half, k);
        }

        /* Build the final number of length 'len'. */
        seq_t val;
        if (len & 1) {
            int k = (len - 1) >> 1;
            val = (H << k) | rev_bits(H >> 1, k);
        } else {
            int k = len >> 1;
            val = (H << k) | rev_bits(H, k);
        }
        cache_append(val);
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;

    /* Initialize cache with the first term (a(1)=0). */
    if (cache_len == 0) {
        cache_cap = 128;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;      /* a(1) */
        cache_len = 1;
    }

    /* Generate additional terms until we have at least 'n' terms. */
    while (cache_len < (size_t)n) {
        generate_length(next_len);
        ++next_len;
    }

    return cache[n - 1];
}
