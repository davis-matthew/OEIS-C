/*
 * A329674  Number of meanders of length n with Motzkin-steps avoiding the
 *          consecutive steps DD.
 * https://oeis.org/A329674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A329674: Number of meanders of length n with Motzkin-steps avoiding DD.
   Recurrence (valid for n ≥ 6):
      (n+1)*a(n) +(-4*n-1)*a(n-1) +(n-2)*a(n-2) +(4*n+1)*a(n-3)
      +(7*n-23)*a(n-4) +2*(n-2)*a(n-5) +2*(-n+5)*a(n-6) = 0
   Initial values:
      a(0)=1, a(1)=2, a(2)=5, a(3)=13, a(4)=34, a(5)=90.
   The function is memoised.  It works for any idx_t n ≥ 0 and returns
   the exact value for all seq_t types that support the required range.
*/

#include <stdlib.h>

/* Cached values of the sequence.  The array is grown geometrically. */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Highest index that has been computed.  The initial value of
   (idx_t)-1 guarantees that the first loop starts with i = 0. */
static idx_t last_computed = (idx_t)-1;

/* Grow the cache array so that it can hold index n (inclusive).  */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;
    size_t new_len = cache_len ? cache_len : 1;
    while (new_len <= (size_t)n)
        new_len <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_len = new_len;
}

/* Compute a(n).  The routine fills the cache up to n and returns the
   requested value. */
seq_t a(idx_t n)
{
    if (n < 0)
        return (seq_t)0;

    ensure_cache(n);

    /* Compute values from (last_computed + 1) up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i == 0)          cache[i] = (seq_t)1;
        else if (i == 1)     cache[i] = (seq_t)2;
        else if (i == 2)     cache[i] = (seq_t)5;
        else if (i == 3)     cache[i] = (seq_t)13;
        else if (i == 4)     cache[i] = (seq_t)34;
        else if (i == 5)     cache[i] = (seq_t)90;
        else { /* recurrence for i >= 6 */
            seq_t N = (seq_t)i;                      /* n as seq_t */
            /* coefficients from the recurrence */
            seq_t coeff1 = 4 * N + 1;                /* (4n+1)   */
            seq_t coeff2 = N - 2;                    /* (n-2)    */
            seq_t coeff3 = 4 * N + 1;                /* (4n+1)   */
            seq_t coeff4 = 7 * N - 23;               /* (7n-23)  */
            seq_t coeff5 = 2 * (N - 2);              /* 2(n-2)   */
            seq_t coeff6 = 2 * (N - 5);              /* 2(n-5)   */

            /* positive part: (4n+1)a(n-1) + 2(n-5)a(n-6) */
            seq_t pos = coeff1 * cache[i - 1] + coeff6 * cache[i - 6];

            /* negative part: (n-2)a(n-2) + (4n+1)a(n-3)
                               + (7n-23)a(n-4) + 2(n-2)a(n-5) */
            seq_t neg = coeff2 * cache[i - 2]
                       + coeff3 * cache[i - 3]
                       + coeff4 * cache[i - 4]
                       + coeff5 * cache[i - 5];

            seq_t numerator = pos - neg;   /* The term is always non‑negative. */
            cache[i] = numerator / (N + 1); /* Division is exact. */
        }
    }

    last_computed = n;
    return cache[n];
}
