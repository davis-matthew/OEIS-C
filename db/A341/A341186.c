/*
 * A341186  Numbers that when divided by the sum of their digits leave 16 as
 *          remainder.
 * https://oeis.org/A341186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1392, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*******************************************************************************
 * Search-based implementation of OEIS A341186:
 *   a(n) = the nth positive integer x with x % sum_of_digits(x) == 16
 *   (offset 1).  The serie is generated lazily and memoised.
 *******************************************************************************/
#include <stdlib.h>

/* Return the nth term (n >= 1).  Behaviour for n < 1 is unspecified.
 * The function builds a cache of the first n terms on demand and re‑uses
 * it for subsequent calls.  The implementation works for any seq_t and
 * idx_t that satisfy the requirements of the compilation environment. */
seq_t a(idx_t n)
{
    /* static cache holding the found terms */
    static seq_t *cache = NULL;   /* array of terms */
    static size_t cap   = 0;      /* current capacity of cache */
    static size_t len   = 0;      /* number of terms stored so far */
    static seq_t last   = 0;      /* last term that satisfied the property */

    if (n < 1)                      /* offset 1 – undefined below */
        return 0;                   /* graceful default */

    /* generate more terms if needed */
    if ((size_t)n > len) {
        seq_t x = last + 1;         /* next integer to test */
        while (len < (size_t)n) {
            /* compute sum of decimal digits of x */
            seq_t sum = 0;
            seq_t t   = x;
            while (t > 0) {
                sum += t % 10;
                t   /= 10;
            }

            /* test the defining condition */
            if (sum > 0 && (x % sum) == 16) {
                /* grow cache if full */
                if (len == cap) {
                    size_t newcap = cap ? (cap << 1) : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                    if (!tmp) return 0;        /* allocation failure → return 0 */
                    cache = tmp;
                    cap   = newcap;
                }
                cache[len++] = x;   /* store found term */
                last = x;           /* remember last successful x */
            }
            ++x;                   /* next candidate */
        }
    }

    /* convert 1‑based index to 0‑based array index */
    return cache[(size_t)n - 1];
}
