/*
 * A306106  Numbers with digits in {0,1,2,3} such that every other digit is
 *          strictly less than its neighbors.
 * https://oeis.org/A306106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A306106: numbers with digits in {0,1,2,3} such that
 * every digit in an even position (1‑based) is strictly less than
 * its adjacent digits.  The terms are listed in increasing
 * numerical order.  The function generates the terms by
 * enumerating all decimal numbers of a given length that satisfy
 * the condition and concatenating the lists for increasing
 * lengths, caching results for fast repeated calls.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache of computed terms */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static int    next_len   = 1;   /* next number of digits to generate */

/* add a value to the cache, growing it if necessary */
static void push_value(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = newp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* generate all valid numbers of the given length and append to cache */
static void generate_len(int len)
{
    uint64_t max = 1ULL << (2 * len);   /* 4^len combinations */
    int digits[32];                     /* enough for any length we expect */
    for (uint64_t i = 0; i < max; ++i) {
        uint64_t tmp = i;
        for (int k = len - 1; k >= 0; --k) {
            digits[k] = tmp & 3ULL;
            tmp >>= 2;
        }

        /* leading zeros are not allowed for lengths > 1 */
        if (len > 1 && digits[0] == 0)
            continue;

        /* check the “every even digit < its neighbors” condition */
        bool ok = true;
        for (int pos = 1; pos < len && ok; ++pos) {
            if ((pos & 1) == 1) {             /* even position in 1‑based */
                if (!(digits[pos] < digits[pos - 1]))
                    ok = false;
                else if (pos + 1 < len &&
                         !(digits[pos] < digits[pos + 1]))
                    ok = false;
            }
        }
        if (!ok)
            continue;

        /* compute the decimal value of the digit sequence */
        seq_t val = 0;
        for (int pos = 0; pos < len; ++pos) {
            val = val * 10 + digits[pos];
        }
        push_value(val);
    }
}

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n < 1, but return 0 safely */
        return 0;

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        generate_len(next_len++);
    }
    return cache[n - 1];
}
