/*
 * A248717  Zeroless numbers k such that k - (sum of digits of k) and k -
 *          (product of digits of k) contain the same distinct digits as k.
 * https://oeis.org/A248717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 32, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the n‑th Zeroless number by a simple brute‑force scan.
 *  Does not use a closed‑form formula; it iterates over all positive
 *  integers, tests the property that k (with no zero digits) satisfies
 *  both k - sum_of_digits(k) and k - product_of_digits(k) have the same
 *  set of distinct digits as k, and caches the results for future
 *  calls. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Return the distinct‑digit bitmask of a non‑negative number. */
static inline uint16_t digit_mask(seq_t v)
{
    uint16_t mask = 0;
    do {
        mask |= 1U << (int)(v % 10);
        v /= 10;
    } while (v);
    return mask;
}

/* Test whether a number k satisfies the Zeroless property. */
static inline bool is_valid(seq_t k)
{
    seq_t tmp = k;
    seq_t sum = 0;
    seq_t prod = 1;
    uint16_t mask_k = 0;

    /* No zero digits allowed. */
    do {
        seq_t d = tmp % 10;
        mask_k |= 1U << (int)d;
        sum += d;
        prod *= d;
        if (d == 0) return false;
        tmp /= 10;
    } while (tmp);

    /* Check k - sum property */
    seq_t a = k - sum;
    if (digit_mask(a) != mask_k) return false;

    /* Check k - product property */
    seq_t b = k - prod;
    if (digit_mask(b) != mask_k) return false;

    return true;
}

/* seq_t a(idx_t n): n-th Zeroless number (n >= 1). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static seq_t next_candidate = 1;   /* next integer to test */

    if (n == 0) return 0;  /* undefined, but harmless */

    /* Grow cache to enough capacity. */
    if (n > cache_cap) {
        size_t new_cap = cache_cap < 16 ? 16 : cache_cap * 2;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate missing terms up to n. */
    for (size_t i = cache_size; i < n; ++i) {
        seq_t k;
        do {
            k = next_candidate++;
        } while (!is_valid(k));
        cache[i] = k;
    }
    cache_size = n;

    return cache[n - 1];
}
