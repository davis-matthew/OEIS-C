/*
 * A381046  Happy repdigit numbers.
 * https://oeis.org/A381046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th happy repdigit number (computed by search)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Helper – sum of the squares of the decimal digits of x. */
static seq_t sum_square_digits(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        int d = (int)(x % 10);
        sum += (seq_t)d * (seq_t)d;
        x   = x / 10;
    }
    return sum;
}

/* Helper – test whether x is a happy number in base 10. */
static bool is_happy(seq_t x)
{
    while (x != 1 && x != 4) {      /* 4 is the only non‑happy cycle ...  */
        x = sum_square_digits(x);
    }
    return x == 1;
}

/* ----- Memoisation state ----- */
static seq_t *cache      = NULL;   /* cache of discovered happy repdigits   */
static idx_t  cache_len  = 0;      /* number of cached entries             */
static size_t cache_cap  = 0;      /* current allocated capacity           */
static idx_t  next_len   = 1;      /* next repdigit length to try          */
static int    next_digit = 1;      /* next digit (1..9) for current length */

/* Append a value to the cache, growing it if necessary. */
static void cache_append(seq_t v)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 32 : cache_cap * 2;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_ptr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = v;
}

/* The sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not defined for offset 0 */

    /* Generate repdigits until we have at least n terms or overflow. */
    while (cache_len < n) {
        seq_t val = 0;
        bool overflow = false;

        /* Build the next repdigit of length next_len made of digit
           next_digit.  Detect overflow on every multiplication/addition. */
        for (idx_t i = 0; i < next_len && !overflow; ++i) {
            /* val = val * 10 */
            seq_t mul10 = val * (seq_t)10;
            if (mul10 / (seq_t)10 != val) {
                overflow = true;
                break;
            }
            val = mul10;

            /* val = val + next_digit */
            seq_t added = val + (seq_t)next_digit;
            if (added < val) {          /* addition overflow */
                overflow = true;
                break;
            }
            val = added;
        }

        if (!overflow && is_happy(val)) {
            cache_append(val);
        }

        /* Advance to the next repdigit to examine. */
        ++next_digit;
        if (next_digit > 9) {
            next_digit = 1;
            ++next_len;
        }

        /* If overflow occurred during this repdigit we cannot
           represent any larger repdigits, so stop. */
        if (overflow) {
            break;
        }
    }

    /* If we ran out of representable values, return 0 (undefined). */
    if (cache_len < n) return 0;
    return cache[n - 1];
}
