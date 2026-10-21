/*
 * A294973  Denominators of the continued fraction convergents to sqrt(7)/2.
 * https://oeis.org/A294973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Denominators of continued‑fraction convergents to sqrt(7)/2
 * A294973
 *
 * Recurrence (for n > 7):
 *   a(n) = 254 * a(n-4) - a(n-8)
 * Initial values:
 *   a(0)=1, a(1)=3, a(2)=31, a(3)=96,
 *   a(4)=223, a(5)=765, a(6)=7873, a(7)=24384
 *
 * The function computes a(n) using the above recurrence and memoises
 * intermediate results in a static array so that calls for any order of
 * indices are fast. It works for the integer built types up to 128 bits
 * and for the arbitrary‑precision type used in the C++ builds.
 */

#include <stdlib.h>

/* Return the n‑th term of A294973.  seq_t and idx_t are defined by the
   build system and may be integer types or an arbitrary‑precision class. */
seq_t a(idx_t n)
{
    /* Cache of previously computed terms */
    static seq_t *cache   = NULL;   /* allocated array of terms */
    static size_t capacity = 0;     /* number of elements allocated */
    static size_t count    = 0;     /* number of elements computed (0…count-1) */

    /* Grow the cache if we need to store more indices */
    if ((size_t)n >= capacity) {
        size_t new_cap = capacity ? capacity << 1 : 8;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        /* realloc may return NULL; cast explicitly for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) {            /* allocation failure */
            free(cache);
            cache = NULL;
            capacity = 0;
            count = 0;
            return (seq_t)0;
        }
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialise the first eight terms once */
    if (count == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)31;
        cache[3] = (seq_t)96;
        cache[4] = (seq_t)223;
        cache[5] = (seq_t)765;
        cache[6] = (seq_t)7873;
        cache[7] = (seq_t)24384;
        count = 8;
    }

    /* Compute missing terms up to n */
    for (size_t i = count; i <= (size_t)n; ++i) {
        if (i < 8) {                 /* already initialised */
            continue;
        }
        /* a(i) = 254 * a(i-4) - a(i-8) */
        cache[i] = (seq_t)254 * cache[i - 4] - cache[i - 8];
    }

    /* Update the number of computed terms */
    count = (size_t)n + 1;

    return cache[(size_t)n];
}
