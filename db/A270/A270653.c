/*
 * A270653  Integers k such that A003266(k) is divisible by k.
 * https://oeis.org/A270653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 241, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   div_by_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS sequence A270653:
   integers k such that the product of the first k Fibonacci numbers
   is divisible by k.  The n-th term is returned by a(n).            */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/div_by_k.h"

/* Static cache of found terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k = 1;

/* Return the n-th element (n>=1) of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while (cache_len < n) {
        if (div_by_k(next_k)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
