/*
 * A398422  Numbers containing at least one zero and divisible by the product
 *          of their nonzero digits.
 * https://oeis.org/A398422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 900, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A398422:
    Numbers containing at least one zero and divisible by the product
    of their non‑zero digits.
    The function returns the n‑th such number, offset 1. */
/*  Computation method:
    A simple search generating positive integers in order,
    storing the found terms in a static dynamic array. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Test if a number satisfies the defining property. */
static bool is_good(seq_t num)
{
    seq_t v = num;          /* keep original value for modulus test */
    seq_t prod = (seq_t)1;  /* product of non‑zero digits */
    bool has_zero = false;

    while (num > (seq_t)0) {
        seq_t d = num % (seq_t)10;
        if (d == (seq_t)0) {
            has_zero = true;
        } else {
            prod *= d;
        }
        num /= (seq_t)10;
    }
    return has_zero && ((v % prod) == (seq_t)0);
}

/*  Main sequence function ----------------------------------------------- */
seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = (seq_t)9;   /* numbers < 10 cannot satisfy property */

    /* Sequence is 1‑based; behaviour for n < 1 is undefined */
    if (n == (idx_t)0) return (seq_t)0;

    /* Extend cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        last_checked++;

        if (is_good(last_checked)) {
            /* Ensure capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_checked;
        }
    }
    return cache[(size_t)n - 1];
}
