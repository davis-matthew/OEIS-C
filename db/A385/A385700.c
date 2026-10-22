/*
 * A385700  Numbers such that when the leftmost digit is moved to the unit's
 *          place the result is divisible by 4.
 * https://oeis.org/A385700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose leftmost digit moved to the units place yields a
   multiple of 4.  Computed by brute‑force search. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global cache of sequence values */
static seq_t *cache = NULL;        /* array of computed terms */
static size_t cap = 0;             /* current capacity of cache */
static idx_t count = 0;            /* number of terms stored in cache */
static seq_t next_candidate = 0;   /* next integer to test */

/* compute 10^e (seq_t) */
static seq_t pow10_local(idx_t e)
{
    seq_t p = 1;
    for (idx_t i = 0; i < e; ++i)
        p *= 10;
    return p;
}

/* number of decimal digits in x (seq_t) */
static idx_t digit_len_local(seq_t x)
{
    idx_t len = 0;
    do {
        ++len;
        x /= 10;
    } while (x > 0);
    return len;
}

/* test whether moving the leftmost digit of x to the end yields a multiple of 4 */
static bool valid_candidate(seq_t x)
{
    idx_t len = digit_len_local(x);
    idx_t k = len - 1;                     /* trailing digit count */
    seq_t p10 = pow10_local(k);
    seq_t d   = x / p10;                   /* leftmost digit */
    seq_t y   = x % p10;                   /* suffix after removing leftmost digit */
    seq_t new_num = y * 10 + d;
    return (new_num % 4ULL) == 0;
}

/* ensure cache has space for at least n items */
static void ensure_capacity(idx_t n)
{
    if (n <= (idx_t)cap)
        return;
    size_t newcap = cap ? cap * 2 : 16;
    while (newcap < (size_t)n)
        newcap <<= 1;
    seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!newptr)
        abort();    /* out of memory */
    cache = newptr;
    cap   = newcap;
}

/* return the n‑th term of the sequence (offset 1) */
seq_t a(idx_t n)
{
    if (n <= count)
        return cache[n - 1];

    ensure_capacity(n);

    while (count < n) {
        /* find next valid integer starting from next_candidate */
        while (true) {
            if (valid_candidate(next_candidate)) {
                cache[count++] = next_candidate;
                ++next_candidate;
                break;
            }
            ++next_candidate;
        }
    }
    return cache[n - 1];
}
