/*
 * A256007  Numbers k satisfying |k + 1 - 2F| <= 1 for some positive Fibonacci
 *          number F.
 * https://oeis.org/A256007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 63, u32 n <= 132, u64 n <= 270,
 *            u128 n <= 546, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n‑th element of the sorted, distinct set
 * { 2*F - 2, 2*F - 1, 2*F | F is a positive Fibonacci number }  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of generated terms */
    static seq_t *cache = NULL;
    static size_t len   = 0;
    static size_t cap   = 0;

    /* Fibonacci state that persists between calls */
    static seq_t fib_prev = 0;
    static seq_t fib_cur  = 1;

    /* initialise cache */
    if (!cache) {
        cap   = 128;                     /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        /* assume allocation succeeds in tests */
    }

    /* generate terms until we have at least (n+1) entries */
    while (len <= (size_t)n) {
        seq_t f = fib_cur;

        /* compute the three values for this Fibonacci number */
        seq_t v[3] = { 2 * f - 2, 2 * f - 1, 2 * f };

        for (int i = 0; i < 3; ++i) {
            seq_t val = v[i];
            /* add val only if it is strictly larger than the last one
               already stored – this skips duplicates and preserves order */
            if (len == 0 || val > cache[len - 1]) {
                if (len == cap) {           /* grow storage if needed */
                    cap <<= 1;
                    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                }
                cache[len++] = val;
            }
        }

        /* advance Fibonacci numbers for next iteration */
        seq_t next = fib_prev + fib_cur;
        fib_prev  = fib_cur;
        fib_cur   = next;
    }

    return cache[(size_t)n];
}
