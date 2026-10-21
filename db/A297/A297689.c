/*
 * A297689  Number of nX3 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 2 or 4 neighboring 1s.
 * https://oeis.org/A297689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A297689: Number of n×3 0..1 arrays in which every 1 has 0, 2 or
   4 neighboring 1s horizontally, diagonally or antidiagonally.
   Recurrence (n≥9):
        a(n) = 5*a(n-1) - 2*a(n-2) + 6*a(n-3) - 52*a(n-4)
               + 23*a(n-5) - 3*a(n-6) + 21*a(n-7) - 9*a(n-8)
   Initial values:
        a(1)=5 , a(2)=26 , a(3)=95 , a(4)=415 ,
        a(5)=1801 , a(6)=7526 , a(7)=32173 , a(8)=137242; */
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* Forward declaration of helper */
static seq_t compute_term(idx_t i);

/* Initial known terms */
static const seq_t base_values[8] = {5ULL, 26ULL, 95ULL, 415ULL,
                                    1801ULL, 7526ULL, 32173ULL, 137242ULL};

/* Cache for computed values: cache[0] is unused */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size */
static idx_t cache_max = 0;    /* largest index with a stored value */



seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n<1; return 0 safely */

    /* Allocate or resize cache if necessary */
    if (n > cache_max) {
        /* Determine required size (indices 0..n) */
        size_t required = (size_t)n + 1;

        if (cache == NULL) {
            cache = (seq_t *)calloc(required, sizeof(seq_t));
            if (!cache) abort();
            cache_cap = required;
        } else if (required > cache_cap) {
            /* Grow to a power of two */
            size_t new_cap = cache_cap;
            if (new_cap == 0) new_cap = 1;
            while (new_cap < required) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            /* Zero the newly allocated part */
            if (new_cap > cache_cap)
                memset((void *)((char *)tmp + cache_cap * sizeof(seq_t)),
                       0, (new_cap - cache_cap) * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Copy the initial values into cache */
        size_t copy_len = (size_t)8;
        if (copy_len > cache_cap - 1) copy_len = cache_cap - 1;
        for (size_t i = 0; i < copy_len; ++i)
            cache[i + 1] = base_values[i];

        /* Compute missing terms only when i ≥ 9 */
        idx_t start = (cache_max > 8 ? cache_max + 1 : 9);
        if (start < 1) start = 1;   /* safety for unlikely cases */
        if (start <= n) {
            for (idx_t i = start; i <= n; ++i)
                cache[i] = compute_term(i);
        }

        /* Update the maximum computed index */
        cache_max = n;
    }

    return cache[n];
}

/* --------------------------------------------------------------------- */
/* Compute a(i) for i ≥ 9 using the recurrence.  Assumes cache[i-1]..i-8
   are already valid.  Uses a 128‑bit intermediate to avoid overflow for
   unsigned integral seq_t.  For arbitrary‑precision seq_t types, the
   operators of seq_t are used directly. */
static seq_t compute_term(idx_t i)
{
#ifdef __cplusplus
    if constexpr (std::is_integral_v<seq_t>) {
        __int128 tmp = 0;
        tmp += (__int128)cache[i-1] * 5;
        tmp -= (__int128)cache[i-2] * 2;
        tmp += (__int128)cache[i-3] * 6;
        tmp -= (__int128)cache[i-4] * 52;
        tmp += (__int128)cache[i-5] * 23;
        tmp -= (__int128)cache[i-6] * 3;
        tmp += (__int128)cache[i-7] * 21;
        tmp -= (__int128)cache[i-8] * 9;
        return (seq_t)tmp;
    } else {
        /* seq_t is non‑integral (e.g. boost::multiprecision) */
        return cache[i-1] * 5
             - cache[i-2] * 2
             + cache[i-3] * 6
             - cache[i-4] * 52
             + cache[i-5] * 23
             - cache[i-6] * 3
             + cache[i-7] * 21
             - cache[i-8] * 9;
    }
#else
    /* C implementation: seq_t is an unsigned integer type */
    __int128 tmp = 0;
    tmp += (__int128)cache[i-1] * 5;
    tmp -= (__int128)cache[i-2] * 2;
    tmp += (__int128)cache[i-3] * 6;
    tmp -= (__int128)cache[i-4] * 52;
    tmp += (__int128)cache[i-5] * 23;
    tmp -= (__int128)cache[i-6] * 3;
    tmp += (__int128)cache[i-7] * 21;
    tmp -= (__int128)cache[i-8] * 9;
    return (seq_t)tmp;
#endif
}
