/*
 * A390827  a(n) = Sum_{k=0..n} binomial(n+2*k+2,n-k) * Fibonacci(k+1).
 * https://oeis.org/A390827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+2*k+2,n-k) * Fibonacci(k+1).
 *            Recurrence: a(n) = 7*a(n-1) - 17*a(n-2) + 23*a(n-3) - 16*a(n-4)
 *            + 6*a(n-5) - a(n-6). Initial values: a(0)=1, a(1)=4, a(2)=14,
 *            a(3)=52, a(4)=202, a(5)=794
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Store the sequence values in a static cache that grows on demand. */
    static seq_t *cache = NULL;   /* pointer to cache array */
    static size_t cap   = 0;      /* allocated capacity */
    static size_t len   = 0;      /* number of values already computed */

    /* Initialisation: allocate space for the first six terms. */
    if (cap == 0) {
        cap = 6;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();          /* unlikely in this small allocation */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)14;
        cache[3] = (seq_t)52;
        cache[4] = (seq_t)202;
        cache[5] = (seq_t)794;
        len = 6;      /* we have values 0..5 ready */
    }

    /* If the caller asks for a value that is already known, simply return it. */
    if ((size_t)n < len) {
        return cache[n];
    }

    /* Ensure the array is large enough. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();        /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute new values up to a(n) using the recurrence. */
    for (size_t i = len; i <= (size_t)n; ++i) {
        /* The recurrence can be safely split into a positive and a negative part,
           preventing underflow on unsigned types. */
        seq_t pos = cache[i-1] * ((seq_t)7)
                  + cache[i-3] * ((seq_t)23)
                  + cache[i-5] * ((seq_t)6);
        seq_t neg = cache[i-2] * ((seq_t)17)
                  + cache[i-4] * ((seq_t)16)
                  +               cache[i-6];
        cache[i] = pos - neg;
    }

    /* Update how many values are known. */
    len = (size_t)n + 1;

    return cache[n];
}
