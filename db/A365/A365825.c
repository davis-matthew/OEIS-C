/*
 * A365825  Number of integer partitions of n that are not of length 2 and do
 *          not contain n/2.
 * https://oeis.org/A365825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A365825 – Number of integer partitions of n that are not
   of length 2 and do not contain n/2.
   Formula:
     a(n) = p(n) - (n-1)/2                                         if n is odd
     a(n) = p(n) - n/2 - p(n/2) + 1                                 if n is even
   where p(k) is the partition number of k (A000041).              */
 
#include <stdlib.h>   /* malloc, realloc, free */
#include <stdint.h>   /* uint64_t */
#include <stddef.h>   /* size_t */
 
/* Helper cast that works in both C and C++ */
#ifdef __cplusplus
#  define REINTERPRET_CAST(type,ptr) reinterpret_cast<type>(ptr)
#else
#  define REINTERPRET_CAST(type,ptr) ((type)(ptr))
#endif
 
/* ------------------------------------------------------------------ */
/* Partition cache: p_cache[i] = p(i)                                  */
/* ------------------------------------------------------------------ */
static seq_t *p_cache = NULL;  /* dynamic array of partition numbers */
static size_t  p_cap    = 0;    /* current allocated size (in elements) */
static idx_t   p_max    = 0;    /* largest index already computed */

/* Ensure p_cache is large enough to hold indices up to `needed` (inclusive). */
static inline void ensure_cache_capacity(idx_t needed)
{
    if (p_cache != NULL && (idx_t)(p_cap - 1) >= needed)
        return;                           /* already sufficient */

    size_t new_cap = (p_cap == 0) ? 1 : p_cap;
    while ((idx_t)(new_cap - 1) < needed)
        new_cap <<= 1;                    /* double until big enough */

    seq_t *tmp = REINTERPRET_CAST(seq_t*,realloc(p_cache,new_cap*sizeof(seq_t)));
    if (tmp == NULL) {                     /* allocation failure */
        free(p_cache);
        exit(EXIT_FAILURE);
    }
    p_cache = tmp;
    p_cap   = new_cap;
}

/* Compute partition numbers up to n if they have not yet been computed
   using Euler's pentagonal‐number recurrence. */
static inline void compute_partitions_upto(idx_t n)
{
    if (p_cache == NULL) {                 /* first time: initialise */
        ensure_cache_capacity(n);
        p_cache[0] = (seq_t)1;             /* p(0) = 1 */
        p_max = 0;
    }
    if (n <= p_max)
        return;                            /* already computed */

    ensure_cache_capacity(n);
    for (idx_t i = p_max + 1; i <= n; ++i) {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            idx_t gp1 = k * (3 * k - 1) / 2;
            if (gp1 > i)
                break;
            seq_t term = p_cache[i - gp1];
            if (k & 1)
                pos += term;
            else
                neg += term;
            idx_t gp2 = k * (3 * k + 1) / 2;
            if (gp2 <= i) {
                term = p_cache[i - gp2];
                if (k & 1)
                    pos += term;
                else
                    neg += term;
            }
        }
        p_cache[i] = pos - neg;
    }
    p_max = n;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    compute_partitions_upto(n);
    seq_t result;

    if ((n & 1) == 0) {                     /* even n */
        result = p_cache[n] - (seq_t)(n / 2);
        result -= p_cache[n / 2];
        result += (seq_t)1;
    } else {                                /* odd n */
        result = p_cache[n] - (seq_t)((n - 1) / 2);
    }
    return result;
}
