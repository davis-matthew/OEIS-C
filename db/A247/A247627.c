/*
 * A247627  Knife numbers (fourth version): n=concat(a,b) is divisible by a+b;
 *          a>0 and b may be zero but not have leading zeros.
 * https://oeis.org/A247627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Knife numbers: integers n that can be split as n = concat(a,b) with a>0,
   b may be zero but must have no leading zeros, and n is divisible by a+b.
   The sequence a(n) enumerates these integers in increasing order,
   starting with 10 (offset 1). */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Count decimal digits of a seq_t value */
static int seq_digit_len(seq_t x)
{
    if (x == 0) return 1;
    int d = 0;
    while (x != 0) {
        d++;
        x /= 10;
    }
    return d;
}

/* Test whether a seq_t value is a knife number */
static bool is_knife_number(seq_t n)
{
    int d = seq_digit_len(n);            /* total number of decimal digits */
    if (d < 2) return false;             /* need at least two digits */

    seq_t pow10 = 10;                    /* 10^(split-length) */
    for (int i = 1; i < d; ++i) {
        seq_t a = n / pow10;
        seq_t b = n % pow10;

        /* Skip splits where b has leading zeros:
           this occurs when the suffix length > 1 and b < 10^(len-1). */
        if (pow10 > 10 && b < pow10 / 10) {
            pow10 *= 10;
            continue;
        }

        seq_t sum = a + b;
        if (sum != 0 && n % sum == 0)
            return true;

        pow10 *= 10;
    }
    return false;
}

/* Memoization state */
static seq_t *knife_cache   = NULL;  /* dynamic array of found values   */
static size_t cache_cap     = 0;     /* allocated capacity of knife_cache */
static size_t cache_count   = 0;     /* number of terms stored          */
static seq_t last_checked   = 0;     /* largest integer examined so far */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset, guard against 0 */

    /* Return cached value if already computed. */
    if (n <= cache_count) return knife_cache[n - 1];

    /* Search forward until we have at least n terms. */
    seq_t candidate = last_checked + 1;
    while (cache_count < n) {
        if (is_knife_number(candidate)) {
            /* Ensure capacity for the new term. */
            if (cache_cap <= cache_count) {
                cache_cap = (cache_cap > 0) ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(knife_cache, cache_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failed – terminate the program. */
                    exit(EXIT_FAILURE);
                }
                knife_cache = tmp;
            }
            knife_cache[cache_count++] = candidate;
        }
        candidate++;
    }

    last_checked = candidate - 1;
    return knife_cache[n - 1];
}
