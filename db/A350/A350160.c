/*
 * A350160  Odd numbers whose Collatz trajectory does not include 5 as a term.
 * https://oeis.org/A350160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th odd integer whose Collatz trajectory never
 *            contains the term 5. The sequence is not given by a simple
 *            closed form, so the function generates terms on demand by
 *            testing consecutive odd numbers
 * Range:     u8 n <= 3, u16 n <= 44, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Generates the n-th term of OEIS A350160.
   n >= 1.  The function is thread‑local by using static state but
   is safe for arbitrary call order. */

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    /* next odd number to examine */
    static seq_t nextodd = 1;

    /* guard against n==0 but callers must use n>=1 */
    if (n == 0) return 0;

    /* Expand cache until it contains at least n terms */
    while (cache_len < n) {
        bool ok = true;
        seq_t x = nextodd;

        /* simulate Collatz steps until reaching 1 */
        while (x != 1) {
            if (x == 5) {      /* 5 appears – reject this odd number */
                ok = false;
                break;
            }
            if (x & 1U) {      /* odd: 3x+1 */
                x = x * 3 + 1;
            } else {           /* even: x/2 */
                x >>= 1;
            }
        }

        if (ok) {               /* store the valid odd number */
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 32;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = nextodd;
        }

        /* advance to next odd candidate */
        nextodd += 2;
    }

    return cache[n - 1];   /* 1‑based indexing */
}
