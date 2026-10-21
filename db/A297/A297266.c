/*
 * A297266  Numbers whose base-8 digits have greater up-variation than
 *          down-variation; see Comments.
 * https://oeis.org/A297266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer whose base‑8 digits have
 *            greater up‑variation than down‑variation (UV > DV). The function
 *            computes the sequence by brute‑force search, caching results
 * Range:     u8 n <= 141, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Static cache for already found terms. */
static seq_t *cache      = NULL;   /* array of found terms          */
static idx_t  cache_size = 0;      /* number of terms stored        */
static idx_t  cache_cap  = 0;      /* capacity of the array         */
static seq_t  next_candidate = 1;  /* candidate to test next        */

/* Ensure the cache has room for at least one more element. */
static void ensure_cache_capacity(void)
{
    if (cache_size + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Test whether a number satisfies UV > DV in its base‑8 representation. */
static bool satisfies(seq_t n)
{
    /* Numbers with fewer than two octal digits have UV = DV = 0. */
    if (n < 8) return false;

    /* Dynamically store octal digits: least significant first. */
    size_t cap = 16;
    unsigned char *digits = (unsigned char *)malloc(cap);
    if (!digits) return false;  /* out of memory – unlikely in tests */

    size_t len = 0;
    seq_t temp = n;
    while (temp > 0) {
        if (len == cap) {
            cap *= 2;
            unsigned char *tmp = (unsigned char *)realloc(digits, cap);
            if (!tmp) { free(digits); return false; }
            digits = tmp;
        }
        digits[len++] = (unsigned char)(temp % 8);
        temp /= 8;
    }

    seq_t dv = 0, uv = 0;
    /* Iterate from most significant to least significant digit. */
    for (size_t i = len; i > 1; --i) {
        seq_t cur  = digits[i - 1];
        seq_t prev = digits[i - 2];
        if (cur > prev)
            dv += cur - prev;
        else if (cur < prev)
            uv += prev - cur;
    }

    free(digits);
    return uv > dv;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    /* Build terms until we have at least n of them. */
    while (cache_size < n) {
        /* Find next qualifying number. */
        while (!satisfies(next_candidate))
            ++next_candidate;

        /* Store the qualifying number. */
        ensure_cache_capacity();
        cache[cache_size++] = next_candidate;
        ++next_candidate;
    }
    return cache[n - 1];
}
