/*
 * A323650  Flower garden sequence (see Comments for precise definition).
 * https://oeis.org/A323650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*A147562(n/2) (n even) a(n) = 3*A147562((n-1)/2) +
 *            A147562(n) - A147562(n-1) (n odd)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises A147562 via the recurrence
   f(0)=0, f(1)=1, f(2k)=4*f(k)+1, f(2k+1)=3*f(k)+f(k+1)+1.
   It uses a simple dynamic table growing by powers of two. */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stddef.h>   /* size_t */

/* Static storage for memoised values of A147562. */
static seq_t *f_values = NULL;        /* array of values */
static unsigned char *f_done = NULL;  /* flag: 1 if value computed */
static size_t f_cap = 0;              /* current array capacity */

/* Ensure the memoisation table can hold index n. */
static void ensure_cap(size_t n)
{
    if (n < f_cap)
        return;
    size_t newcap = f_cap ? f_cap * 2 : 1;
    while (n >= newcap)
        newcap <<= 1;

    seq_t *newv = (seq_t *)realloc(f_values, newcap * sizeof(seq_t));
    if (!newv) abort();          /* allocation failure */
    unsigned char *newd = (unsigned char *)realloc(f_done,
                                                   newcap * sizeof(unsigned char));
    if (!newd) { free(newv); abort(); }

    /* initialise the newly allocated slots */
    for (size_t i = f_cap; i < newcap; ++i) {
        newv[i] = 0;
        newd[i] = 0;
    }

    f_values = newv;
    f_done   = newd;
    f_cap    = newcap;
}

/* Recursive memoised evaluation of A147562. */
static seq_t get_f(idx_t n)
{
    ensure_cap((size_t)n);
    if (f_done[(size_t)n])
        return f_values[(size_t)n];

    seq_t val;
    if (n == 0)
        val = 0;
    else if (n == 1)
        val = 1;
    else if ((n & 1ULL) == 0ULL)       /* even n = 2k */
        val = 4 * get_f(n >> 1) + 1;
    else                               /* odd n = 2k+1 */
        val = 3 * get_f(n >> 1)
            + get_f((n >> 1) + 1)
            + 1;

    f_values[(size_t)n] = val;
    f_done[(size_t)n]   = 1;
    return val;
}

/* Public entry point for OEIS sequence A323650. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    if ((n & 1ULL) == 0ULL)                /* even n */
        return 3 * get_f(n >> 1);

    /* odd n */
    return 3 * get_f(n >> 1) + get_f(n) - get_f(n - 1);
}
