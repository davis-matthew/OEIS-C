/*
 * A354081  Positive integers k such that the first digit of k is divisible by
 *          the product of all the remaining digits of k.
 * https://oeis.org/A354081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer k such that the first * decimal
 *            digit of k is divisible by the product of the remaining digits.
 *            * The function computes terms by incremental search and
 *            memoisation
 * Range:     u8 n <= 36, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Predicate: true if k satisfies the condition. */
static bool is_term(seq_t k)
{
    /* Single‑digit numbers are always valid. */
    if (k < 10)
        return true;

    /* Count decimal digits of k. */
    seq_t temp = k;
    int digits = 0;
    while (temp > 0) {
        temp /= 10;
        digits++;
    }

    /* Compute the product of the remaining digits (skip the most
     * significant one).  If any digit is 0 or product exceeds 9
     * (impossible for a divisor of the first digit), reject. */
    seq_t prod = 1;
    int accounted = 0;
    temp = k;
    while (accounted < digits - 1) {
        seq_t digit = temp % 10;
        if (digit == 0)
            return false;
        prod *= digit;
        if (prod > 9)      /* it can’t divide the first digit (≤9) */
            return false;
        temp /= 10;
        accounted++;
    }

    /* Now temp holds the first (most significant) digit. */
    seq_t first = temp;
    return (first % prod) == 0;
}

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next = 1;    /* next candidate number */

    if (n == 0)
        return 0;            /* undefined for n < 1 */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        if (is_term(next)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();   /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next;
        }
        next++;   /* advance to the next candidate */
    }
    return cache[(size_t)n - 1];
}
