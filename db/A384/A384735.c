/*
 * A384735  Numbers that are prime or end in a prime number (of any length).
 * https://oeis.org/A384735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer that is either prime or has a
 *            prime decimal suffix
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */

static bool has_prime_suffix_or_is_prime(seq_t x) {
    /* Prime numbers themselves are accepted. */
    if (is_prime_big(x))
        return true;

    /* Check each suffix: last 1 digit, last 2 digits, ..., last d-1 digits. */
    seq_t mod = 10;
    while (mod <= x) {
        seq_t suffix = x % mod;
        if (suffix >= (seq_t)2 && is_prime_big(suffix))
            return true;
        mod *= (seq_t)10;
    }
    return false;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* stored sequence values */
    static size_t capacity = 0;   /* current allocation size */
    static size_t size = 0;       /* number of values stored */
    static seq_t last_checked = 1;/* last integer examined */

    if (n == 0)   /* undefined for n < 1, return 0 for safety */
        return (seq_t)0;

    /* Extend the cache until we have at least n terms. */
    while (size < n) {
        ++last_checked;                   /* next candidate integer */
        if (has_prime_suffix_or_is_prime(last_checked)) {
            if (size >= capacity) {
                capacity = capacity ? capacity * 2 : 128;
                cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
                if (!cache) exit(1);          /* allocation failure */
            }
            cache[size++] = last_checked;   /* store the term */
        }
    }
    return cache[n - 1];
}
