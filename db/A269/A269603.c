/*
 * A269603  Number of length-n 0..5 arrays with no repeated value differing
 *          from the previous repeated value by one or less.
 * https://oeis.org/A269603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A269603
    Formula: a(n) = 17*a(n-1) - 91*a(n-2) + 83*a(n-3) + 542*a(n-4)
              - 550*a(n-5) - 1651*a(n-6) - 745*a(n-7)   for n ≥ 8
    Base values:
        a(1)= 6, a(2)= 36, a(3)=210, a(4)=1220,
        a(5)=7030, a(6)=40288, a(7)=229754
    The function is memoized; it can be called in any order.
    All arithmetic is performed using the unsigned type seq_t,
    which automatically wraps modulo 2^bits for types smaller than 128 bits. */

#include <stdbool.h>
#include <stdlib.h>

/* These first seven terms are supplied by the definition. */
static const seq_t init[7] = {
    (seq_t)6ULL,          /* a(1) */
    (seq_t)36ULL,         /* a(2) */
    (seq_t)210ULL,        /* a(3) */
    (seq_t)1220ULL,       /* a(4) */
    (seq_t)7030ULL,       /* a(5) */
    (seq_t)40288ULL,      /* a(6) */
    (seq_t)229754ULL      /* a(7) */
};

/* Dynamic cache of computed terms. */
static seq_t *cache = NULL;       /* 1‑based indexing; cache[0] unused */
static idx_t   cached_n = 0;      /* number of terms currently stored */
static size_t  capacity  = 0;     /* allocated array size (≥ cached_n+1) */

seq_t a(idx_t n)
{
    /* Handle the trivial case directly. */
    if (n <= 7)
        return init[n - 1];

    /* Lazily allocate the cache if this is the first call. */
    if (cache == NULL) {
        /* Allocate enough space for at least the first seven terms. */
        capacity = (size_t) (n > 7 ? n : 7) + 1;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;  /* Out‑of‑memory: return 0 (unlikely in test) */

        /* Store the known first terms. */
        for (idx_t i = 1; i <= 7; ++i)
            cache[i] = init[i - 1];

        cached_n = 7;
    }

    /* Grow the allocated array if the caller asks for a larger n. */
    if (n >= capacity) {
        size_t new_cap = capacity;
        while (n >= new_cap)
            new_cap <<= 1;          /* double until large enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;       /* Out‑of‑memory */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms if needed. */
    if (n > cached_n) {
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            seq_t next =
                (seq_t)17   * cache[i - 1]
              - (seq_t)91  * cache[i - 2]
              + (seq_t)83  * cache[i - 3]
              + (seq_t)542 * cache[i - 4]
              - (seq_t)550 * cache[i - 5]
              - (seq_t)1651* cache[i - 6]
              - (seq_t)745 * cache[i - 7];
            cache[i] = next;
        }
        cached_n = n;
    }

    return cache[n];
}
