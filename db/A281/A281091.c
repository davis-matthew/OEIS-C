/*
 * A281091  Numbers beginning and ending with their digital root in decimal
 *          representation.
 * https://oeis.org/A281091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 738, u32 n <= 9654, u64 n <= 9654,
 *            u128 n <= 9654, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A281091: Numbers beginning and ending with their digital root.
    This implementation computes terms on demand by scanning integers
    and validating the required property.  A static cache stores all
    terms that have been generated so far, allowing calls in any order. */
#include <stdlib.h>
#include <stdbool.h>

/* Test whether integer v starts and ends with its digital root. */
static bool is_valid(idx_t v)
{
    idx_t dr;
    if (v == 0)
        dr = 0;
    else
        dr = 1 + (v - 1) % 9;          /* digital root of v */

    idx_t last = v % 10;               /* last digit */

    idx_t first = v;
    while (first >= 10)
        first /= 10;                   /* first digit */

    return first == dr && last == dr;
}

seq_t a(idx_t n)
{
    /* Static cache of generated terms */
    static seq_t *cache    = NULL;
    static size_t  cap     = 0;
    static size_t  len     = 0;
    static idx_t   next_num = 0;      /* next integer to test */

    if (n == 0)                     /* undefined, return 0 */
        return 0;

    /* Generate terms until we have at least n of them */
    while (len < n) {
        if (is_valid(next_num)) {
            if (len == cap) {        /* grow cache if needed */
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = (seq_t)next_num;
        }
        ++next_num;
    }
    return cache[n - 1];
}
