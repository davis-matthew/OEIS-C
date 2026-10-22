/*
 * A376314  Nonnegative numbers k with digital sum d such that any positive
 *          integer <= d is the sum of contiguous digits of k.
 * https://oeis.org/A376314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Compute OEIS A376314 by brute‑force search in increasing order
 *            and testing the contiguous‑digit‑sum property of each k
 * Range:     u8 n <= 22, u16 n <= 501, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/get_digits.h"

#define MAX_DIGITS 200     /* enough for any term needed in the tests */
#define MAX_SUM    2000    /* largest digit sum we consider */

/* Return true iff k satisfies the definition of A376314. */
static bool is_good_k(seq_t k)
{
    /* Obtain the decimal digits of k in an array, and its length. */
    uint8_t digits[MAX_DIGITS];
    size_t len;
    get_digits(k, digits, &len);

    /* Compute the digit sum. */
    size_t dsum = 0;
    for (size_t i = 0; i < len; ++i)
        dsum += digits[i];

    /* k = 0 is a special case, it has digit sum 0 and is automatically good. */
    if (dsum == 0)
        return true;

    if (dsum > MAX_SUM)   /* too large for our static buffer */
        return false;

    /* Mark all contiguous sums that appear. */
    bool seen[MAX_SUM + 1];
    memset(seen, 0, (dsum + 1) * sizeof(bool));

    for (size_t i = 0; i < len; ++i) {
        size_t s = 0;
        for (size_t j = i; j < len; ++j) {
            s += digits[j];
            if (s <= dsum)
                seen[s] = true;
        }
    }

    /* Every integer from 1 up to dsum must be representable. */
    for (size_t v = 1; v <= dsum; ++v)
        if (!seen[v])
            return false;

    return true;
}

/* Cache of computed sequence terms. */
static seq_t *cache     = NULL;
static size_t  cache_sz = 0;
static size_t  cache_cap = 0;
static seq_t  next_k = 0;

/* Return the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined, but handle gracefully */

    while (cache_sz < n) {
        if (is_good_k(next_k)) {
            if (cache_sz == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    return 0;   /* allocation failure – unlikely in tests */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = next_k;
        }
        /* Advance to the next candidate, accounting for potential overflow. */
        next_k = next_k + 1;
    }
    return cache[n - 1];
}
