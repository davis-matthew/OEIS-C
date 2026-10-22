/*
 * A321336  Expansion of Product_{k>=0} (1 - x^(2^k))^(2^(k+1)).
 * https://oeis.org/A321336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 32, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A321336: Expansion of Product_{k>=0} (1 - x^(2^k))^(2^(k+1))

   Recurrence used:  
      let b[i] = a[i/2]          if i even
               = -a[(i-1)/2]     if i odd
      then  a[n] = Σ_{i=0}^{n} b[i] * b[n-i]
   with a[0] = 1.
   The values of a(n) are computed incrementally and cached in a
   static array so that arbitrary order calls work correctly.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static arrays for memoized values */
    static seq_t *vals = NULL;   /* a[] values  : sequence values */
    static seq_t *buf  = NULL;   /* b[] values  : auxiliary values */
    static idx_t  max_n = (idx_t)-1;  /* highest index computed */

    /* Initial allocation: a[0] = 1, b[0] = 1 */
    if (max_n == (idx_t)-1) {
        max_n = 0;
        vals = (seq_t *)malloc(1 * sizeof(seq_t));
        buf  = (seq_t *)malloc(1 * sizeof(seq_t));
        if (!vals || !buf) return (seq_t)0;      /* allocation failed */
        vals[0] = (seq_t)1;
        buf[0]  = vals[0];
    }

    /* If requested n already computed, return it */
    if (n <= max_n) return vals[n];

    /* Grow arrays to accommodate up to n */
    size_t new_size = (size_t)n + 1;
    if (new_size > (size_t)max_n + 1) {
        seq_t *tmp = (seq_t *)realloc(vals, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        vals = tmp;
        tmp = (seq_t *)realloc(buf, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        buf  = tmp;
    }

    /* Compute all missing terms from max_n+1 up to n */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        /* Compute auxiliary value b[k] */
        if ((k & 1) == 0) {          /* even k */
            buf[k] = vals[k >> 1];
        } else {                     /* odd k */
            buf[k] = (seq_t)-1 * vals[(k - 1) >> 1];
        }

        /* Compute a[k] = Σ_{i=0}^{k} b[i] * b[k-i] */
        seq_t sum = 0;
        for (idx_t i = 0; i <= k; ++i) {
            sum += buf[i] * buf[k - i];
        }
        vals[k] = sum;
    }

    max_n = n;
    return vals[n];
}
