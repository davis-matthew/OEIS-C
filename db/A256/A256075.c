/*
 * A256075  Non-palindromic balanced numbers (in base 10).
 * https://oeis.org/A256075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2213, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9180 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non‑palindromic balanced numbers (base 10).
   The function generates the sequence by brute‑force scanning from
   1000 upwards, testing each number for balancedness and
   non‑palindrome property. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true if x (unsigned long long) is balanced in base 10. */
static bool balanced_ull(unsigned long long x)
{
    int digits[20];
    int len = 0;
    unsigned long long t = x;
    while (t > 0) {
        digits[len++] = t % 10;
        t /= 10;
    }
    if (len == 0) {          /* number 0 (not used here) */
        digits[0] = 0;
        len = 1;
    }

    /* Compute weighted sum difference multiplied by 2 to keep integers.
       weight2 = 2*i - len + 1, where i is 0‑based from the left. */
    long long diff = 0;   /* signed to accumulate potential negative values */
    for (int i = 0; i < len; ++i) {
        int d = digits[len - 1 - i]; /* most significant digit first */
        int weight2 = 2 * i - len + 1;
        diff += (long long)d * weight2;
    }
    return diff == 0;
}

/* Return true if x is a palindrome in base 10. */
static bool is_palindrome_ull(unsigned long long x)
{
    unsigned long long rev = 0, tmp = x;
    while (tmp > 0) {
        rev = rev * 10 + (tmp % 10);
        tmp /= 10;
    }
    return rev == x;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Cache of already computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static unsigned long long current = 999;   /* last tested candidate */

    if (cache_size < (size_t)n) {
        /* Ensure cache capacity. */
        size_t new_size = (size_t)n;
        seq_t *new_cache = mem_resize(seq_t, cache, new_size);
        if (!new_cache)
            return (seq_t)0;   /* graceful failure on allocation error */
        cache = new_cache;
    }

    while (cache_size < (size_t)n) {
        ++current;
        if (!balanced_ull(current))
            continue;
        if (is_palindrome_ull(current))
            continue;   /* skip palindromic numbers */
        cache[cache_size++] = (seq_t)current;
    }

    return cache[n - 1];
}
