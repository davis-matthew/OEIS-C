/*
 * A373097  a(n) = a[n/r] + a[n/r^2] + a[n/r^3] + ... , where a(0) = 0, a(1) =
 *          1, r = 3/2, and [ ] = floor.
 * https://oeis.org/A373097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 57, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(1) = 1,
   a(n) = Σ_{k ≥ 1} a( floor( n * 2^k / 3^k ) )   (stop when the floor is 0)  */

#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Memoization cache                                                    */
/* --------------------------------------------------------------------- */
static seq_t *cache = NULL;   /* array of computed a(n)                        */
static bool *used  = NULL;   /* used[i] is true iff cache[i] is valid        */
static size_t cap  = 0;      /* current capacity of the arrays               */

/* Ensure the cache can hold index n (and initialize used flags).       */
static void ensure_cache(idx_t n)
{
    if (n >= cap) {
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        used  = (bool  *)realloc(used,  new_cap * sizeof(bool));
        for (size_t i = cap; i < new_cap; ++i) used[i] = false;
        cap = new_cap;
    }
}

/* --------------------------------------------------------------------- */
/*  Sequence evaluation                                                   */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    ensure_cache(n);
    if (used[n]) return cache[n];

    seq_t sum = 0;
    seq_t pow2 = (seq_t)2;   /* 2^k */
    seq_t pow3 = (seq_t)3;   /* 3^k */

    while (true) {
        /* m = floor(n * 2^k / 3^k) = floor( (seq_t)n * pow2 / pow3 )   */
        idx_t m = (idx_t)(((seq_t)n * pow2) / pow3);
        if (m == 0) break;
        sum += a(m);

        /* advance to the next power: multiply by 2 and 3 respectively */
        pow2 *= (seq_t)2;
        pow3 *= (seq_t)3;
    }

    cache[n] = sum;
    used[n]  = true;
    return sum;
}
