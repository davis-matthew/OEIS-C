/*
 * A280641  Numbers k such that k^3 has an odd number of digits and the middle
 *          digit is 1.
 * https://oeis.org/A280641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 2006, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence of integers k such that k^3 has an odd number of digits and the middle digit is 1. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_k = 1;

static void grow_cache(void)
{
    size_t new_cap = cache_cap == 0 ? 8 : cache_cap << 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) {
        abort(); /* allocation failure */
    }
    cache = tmp;
    cache_cap = new_cap;
}

static bool good_candidate(seq_t k)
{
    seq_t cube = k * k * k;
    seq_t t = cube;
    unsigned char digits[256];
    size_t len = 0;

    if (t == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (t > 0) {
            digits[len++] = (unsigned char)(t % 10);
            t /= 10;
        }
    }
    /* odd number of digits and middle digit 1 */
    return (len & 1) && digits[len >> 1] == 1;
}

seq_t a(idx_t n)
{
    /* index offset: a(1) is the first term listed */
    if (n == 0) return 0;             /* undefined for offset 1, but keep safe */

    while (cache_len < n) {
        seq_t k = (seq_t)next_k;
        if (good_candidate(k)) {
            if (cache_len == cache_cap) {
                grow_cache();
            }
            cache[cache_len++] = k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
