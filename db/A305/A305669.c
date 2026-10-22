/*
 * A305669  Take the sum of the digits of a number, put it at the left side
 *          and delete the same number of digits at the right side. Repeat the
 *          process. Sequence lists numbers that reach themselves after some
 *          steps.
 * https://oeis.org/A305669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the n‑th integer that returns to itself after repeated
 *            digit‑sum‑left transformations (i.e., it lies on a cycle of the
 *            map). The transformation f on a number x is defined by s =
 *            digit_sum(x) k = digit_len(x) d = digit_len(s) f(x) = (s * 10**k
 *            + x) / 10**d where division is integer division
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   digit_len, digit_sum, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdbool.h>  /* bool */
#include "../utils/digit_sum.h"
#include "../utils/digit_len.h"
#include "../utils/pow_uint.h"

/* Next value of the digit‑sum‑left transformation. */
static inline seq_t next_in_cycle(seq_t n)
{
    seq_t sum = digit_sum(n);
    int    k   = digit_len(n);
    int    d   = digit_len(sum);

    seq_t pow_k = pow_uint(10, (idx_t)k);
    seq_t pow_d = pow_uint(10, (idx_t)d);

    return (sum * pow_k + n) / pow_d;
}

/* Return true iff n lies on a cycle of the transformation. */
static bool in_cycle(seq_t n)
{
    seq_t tortoise = next_in_cycle(n);
    seq_t hare     = next_in_cycle(next_in_cycle(n));

    /* Find first equality (Floyd). */
    while (tortoise != hare) {
        tortoise = next_in_cycle(tortoise);
        hare     = next_in_cycle(next_in_cycle(hare));
    }

    /* Find start of cycle (mu). */
    tortoise = n;
    while (tortoise != hare) {
        tortoise = next_in_cycle(tortoise);
        hare     = next_in_cycle(hare);
    }

    /* n is on the cycle iff mu == 0, i.e., tortoise returned to n. */
    return tortoise == n;
}

/* Return the n‑th smallest integer that lies on a cycle. */
seq_t a(idx_t n)
{
    /* Static cache of discovered cycle numbers. */
    static seq_t *cache = NULL;   /* dynamic array of seq_t */
    static size_t cache_cap = 0;  /* allocated capacity */
    static size_t cache_size = 0; /* number of elements stored */
    static idx_t next_scan = 0;   /* next integer to test */

    /* Grow the cache until we have at least n+1 elements. */
    while (cache_size <= n) {
        seq_t cand = (seq_t)next_scan;   /* candidate number */

        if (in_cycle(cand)) {
            /* Make sure there is room for another element. */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               new_cap * sizeof(seq_t));
                if (!tmp) abort(); /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }
        ++next_scan;
    }

    return cache[n];
}
