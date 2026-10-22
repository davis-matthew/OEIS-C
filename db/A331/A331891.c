/*
 * A331891  Negabinary palindromes: nonnegative numbers whose negabinary
 *          expansion (A039724) is palindromic.
 * https://oeis.org/A331891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 425, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th nonnegative integer whose base –2 representation is a palindrome. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* External integer types supplied by the build system */
static seq_t *cache = NULL;          /* sorted cache of found palindromes */
static size_t cache_len = 0;         /* number of elements in the cache   */
static size_t cache_cap = 0;         /* current allocation capacity      */
static idx_t next_len = 1;           /* length of the next palindromes to generate */

static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

static void cache_append(seq_t v)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = v;
}

/* Generate all palindromic negabinary numbers of a given length.
   Length is at least 1.  Non‑negative values are appended to the cache. */
static void generate_len(idx_t len)
{
    idx_t half = (len - 1) / 2;                /* number of free bits before mirroring */
    size_t free_bits = 1ULL << half;           /* 2^half possibilities          */

    uint8_t bits[64];                          /* accommodate lengths up to 64   */
    bits[0] = 1;                               /* leading bit must be 1          */
    bits[len - 1] = 1;                         /* trailing bit must be 1          */

    for (size_t mask = 0; mask < free_bits; ++mask) {
        /* set the free bits and mirror them */
        for (idx_t j = 0; j < half; ++j) {
            uint8_t b = (mask >> j) & 1;
            bits[1 + j] = b;
            bits[len - 2 - j] = b;
        }

        if (len & 1) {                         /* odd length – middle bit free   */
            for (uint8_t mid = 0; mid < 2; ++mid) {
                bits[half] = mid;
                __int128 val = 0;
                for (idx_t i = 0; i < len; ++i)
                    val = val * (__int128)(-2) + bits[i];
                if (val >= 0)
                    cache_append((seq_t)val);
            }
        } else {                               /* even length – middle fixed      */
            __int128 val = 0;
            for (idx_t i = 0; i < len; ++i)
                val = val * (__int128)(-2) + bits[i];
            if (val >= 0)
                cache_append((seq_t)val);
        }
    }
}

/* Make sure the cache contains at least 'needed' terms. */
static void ensure_cache(idx_t needed)
{
    /* Base case: add 0 as the first term (a(1)=0) */
    if (cache_len == 0)
        cache_append((seq_t)0);

    while (cache_len < (size_t)needed) {
        generate_len(next_len);
        ++next_len;

        /* Keep the cache sorted and free duplicates. */
        qsort(cache, cache_len, sizeof(seq_t), cmp_seq_t);
        size_t uniq = 0;
        for (size_t i = 0; i < cache_len; ++i) {
            if (uniq == 0 || cache[i] != cache[uniq - 1])
                cache[uniq++] = cache[i];
        }
        cache_len = uniq;
    }
}

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* not part of the sequence */
    ensure_cache(n);
    return cache[n - 1];
}
