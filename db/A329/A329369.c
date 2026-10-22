/*
 * A329369  Number of permutations of {1,2,...,m} with excedance set
 *          constructed by taking m-i (0 < i < m) if b(i-1) = 1 where
 *          b(k)b(k-1)...b(1)b(0) (0 <= k < m-1) is the binary expansion of n.
 * https://oeis.org/A329369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 431, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS sequence A329369  
   Recurrence (OEIS notation):  
     a(0) = 1  
     a(2n+1) = a(n)  
     a(2n)   = a(n) + a(n-2^f(n)) + a(2n-2^f(n))   (n > 0)  
   where f(n) = A007814(n) is the exponent of the largest power of two dividing n.  
   The implementation builds the values iteratively, using a growable cache
   that works with both built‑in integral types and arbitrary‑precision
   integer classes.
*/

#include <stdlib.h>   /* malloc, realloc, exit */
#include <stdint.h>   /* uint64_t */
#include <stdbool.h>  /* bool */

static void ensure_cap(idx_t n, seq_t **data, size_t *cap)
{
    size_t new_cap = *cap ? *cap : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    if (new_cap <= *cap) return;

#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < *cap; ++i) tmp[i] = (*data)[i];
    delete [] *data;
#else
    seq_t *tmp = realloc(*data, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);   /* allocation failure */
#endif
    *data = tmp;
    *cap  = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* make sure cache can hold index n */
    ensure_cap(n, &cache, &cap);

    if (n == 0) {                       /* base case */
        cache[0] = (seq_t)1;
        return cache[0];
    }

    /* initialise the first value if not already done */
    if (cache[0] == 0) cache[0] = (seq_t)1;

    /* iteratively compute a(i) for i = 1 .. n */
    for (idx_t i = 1; i <= n; ++i) {
        if (i & 1) {                     /* odd: a(2k+1) = a(k) */
            cache[i] = cache[i >> 1];
        } else {                         /* even: use recurrence */
            idx_t m   = i >> 1;                       /* m = n/2    */
            /* compute the largest power of two dividing m */
            idx_t low = __builtin_ctzll((unsigned long long)m);
            idx_t pow = ((idx_t)1) << low;            /* 2^v2(m)    */
            cache[i] = cache[m]
                     + cache[m - pow]
                     + cache[i - pow];
        }
    }
    return cache[n];
}
