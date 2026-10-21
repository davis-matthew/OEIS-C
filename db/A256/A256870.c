/*
 * A256870  Numbers divisible by prime(d+1) for each digit d of their base-10
 *          representation.
 * https://oeis.org/A256870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th nonnegative integer such that each decimal
 *            digit d of a(n) divides the integer by prime(d+1). The sequence
 *            starts with a(1)=0
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/get_prime.h"

/* Cache of already found terms.  */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* allocated capacity */
static seq_t next_candidate = 0;

/* Ensure that the cache can hold at least 'needed' elements.  */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    /* Cast for C++ compatibility.  C allows implicit conversion. */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* Test whether a given number satisfies the divisibility condition.  */
static bool satisfies(seq_t x)
{
    if (x == 0) return true;          /* 0 has digit 0 → prime(1)=2, and 0 % 2 == 0 */
    seq_t t = x;
    while (t != 0) {
        uint8_t digit = (uint8_t)(t % (seq_t)10);
        seq_t prime = (seq_t)get_prime((idx_t)digit + 1);
        if (x % prime != 0) return false;
        t /= (seq_t)10;
    }
    return true;
}

/* Return the n‑th term (1‑based) of the sequence.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for index 0, but harmless */
    while (cache_len < n) {
        ensure_cache_capacity(cache_len + 1);
        if (satisfies(next_candidate)) {
            cache[cache_len++] = next_candidate;
        }
        next_candidate++;
    }
    return cache[n - 1];
}
