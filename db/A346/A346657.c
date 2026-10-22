/*
 * A346657  Numbers that are not divisible by the product of their nonzero
 *          digits.
 * https://oeis.org/A346657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers that are not divisible by the product of their nonzero digits (OEIS A346657).
   Computed by iterating successive integers and selecting those that do not satisfy
   n % product_of_nonzero_digits(n) == 0. */

#include <stdlib.h>   /* for malloc, realloc */

static seq_t *cache = NULL;       /* dynamically growing array of terms */
static size_t cache_cap = 0;      /* allocated capacity of the array      */
static size_t cache_len = 0;      /* number of terms stored currently    */
static uint64_t next_int = 1;     /* next integer to test                */

/* Return nth term of the sequence (1‑based index).  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, guard against spurios calls */

    /* Extend the cache until we have at least n terms. */
    while (cache_len < n) {
        uint64_t tmp = next_int;
        seq_t prod = 1;          /* product of non‑zero digits */

        /* Compute product of non‑zero digits of next_int. */
        while (tmp > 0) {
            uint64_t d = tmp % 10;
            if (d != 0)
                prod *= (seq_t)d;
            tmp /= 10;
        }

        /* If product is non‑zero and next_int is NOT divisible by it, keep it. */
        if (prod != (seq_t)0 && ((seq_t)next_int % prod) != (seq_t)0) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap ? cache_cap * 2 : 1);
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_int;
        }
        ++next_int;   /* move to the next integer to test */
    }

    return cache[n - 1];
}
