/*
 * A339999  Squares that are divisible by both the sum of their digits and the
 *          product of their nonzero digits.
 * https://oeis.org/A339999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A339999:  squares divisible by the sum of their digits
 * and by the product of their non‑zero digits.
 * The routine performs an incremental brute‑force search, caching every
 * found term so that calls to a(n) for arbitrary n (even out of order)
 * return immediately once the range has been generated.  */
#include <stdlib.h>
#include <stdbool.h>

/* Helper: test whether a square satisfies the conditions. */
static bool good_square(seq_t s)
{
    seq_t temp = s;
    uint64_t sum = 0;              /* sum of digits – fits in 64‑bit */
    seq_t prod = 1;                /* product of non‑zero digits */

    while (temp > (seq_t)0) {
        uint64_t digit = (uint64_t)(temp % (seq_t)10);
        temp /= (seq_t)10;

        sum += digit;
        if (digit != 0) {
            /* avoid overflow: prod * digit <= s if prod <= s / digit */
            if (prod > s / digit) {
                prod = s + (seq_t)1; /* flag overflow */
                break;
            }
            prod *= (seq_t)digit;
        }
    }

    if (s % sum != (seq_t)0)          return false;   /* not divisible by sum of digits */
    if (prod > s)                     return false;   /* product cannot divide the square */
    if (s % prod != (seq_t)0)          return false;   /* not divisible by product of non‑zero digits */
    return true;
}

/* Ensure cache can hold at least 'needed' elements. */
static void ensure_cache_cap(size_t needed, seq_t **c, size_t *cap)
{
    if (*cap == 0) {
        *cap = 4;
        *c   = (seq_t *)malloc(*cap * sizeof(seq_t));
        if (*c == NULL) abort();
    } else if (needed > *cap) {
        size_t new_cap = *cap * 2;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(*c, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        *c = tmp;
        *cap = new_cap;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static cache of found squares. */
    static seq_t *cache    = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t  last_k = 0;          /* last k whose square was examined */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        last_k++;
        seq_t k = (seq_t)last_k;
        seq_t s = k * k;               /* square of k */

        if (good_square(s)) {
            ensure_cache_cap(cache_len + 1, &cache, &cache_cap);
            cache[cache_len++] = s;
        }
    }

    return cache[(size_t)n - 1];
}
