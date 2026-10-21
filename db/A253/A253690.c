/*
 * A253690  Numerators of approximation to 2^(1/3) by Halley's method after n
 *          iterations.
 * https://oeis.org/A253690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the numerator of the n‑th Halley approximation to 2^(1/3).
    Recurrence (for n>0):
      Let a = a(n‑1), b = den(n‑1).
      x = a * ( a³ + 4*b³ )
      y = 2 * b * ( a³ + b³ )
      a(n) = x / gcd(x,y)
      b(n) = y / gcd(x,y)
    with a(0) = b(0) = 1.  All operations are performed in the integer type
    seq_t.  The function caches results so that calls in any order are fast.   */
#include <stdlib.h>
#include "../utils/gcd.h"

#ifdef __cplusplus
#define ALLOC(type, n) reinterpret_cast<type*>(malloc(sizeof(type)*(n)))
#define RE_ALLOC(ptr, n) reinterpret_cast<seq_t*>(realloc((void*)(ptr), sizeof(seq_t)*(n)))
#else
#define ALLOC(type, n) ((type*)malloc(sizeof(type)*(n)))
#define RE_ALLOC(ptr, n) ((seq_t*)realloc((void*)(ptr), sizeof(seq_t)*(n)))
#endif

static seq_t *num_array = NULL;  /* numerators a(n)   */
static seq_t *den_array = NULL;  /* denominators b(n)*/
static idx_t  max_computed = 0;
static size_t capacity    = 0;

/* Ensure values up to index n are computed and cached. */
static void ensure_computed(idx_t n)
{
    if (num_array == NULL) {
        capacity = (size_t)(n + 1);
        if (capacity == 0) capacity = 1;
        num_array = ALLOC(seq_t, capacity);
        den_array = ALLOC(seq_t, capacity);
        num_array[0] = (seq_t)1;
        den_array[0] = (seq_t)1;
        max_computed = 0;
    } else if ((size_t)(n + 1) > capacity) {
        capacity = (size_t)(n + 1);
        num_array = RE_ALLOC(num_array, capacity);
        den_array = RE_ALLOC(den_array, capacity);
    }

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        idx_t prev = i - 1;
        seq_t aPrev = num_array[prev];
        seq_t bPrev = den_array[prev];

        seq_t a3 = aPrev * aPrev * aPrev;          /* aPrev³ */
        seq_t b3 = bPrev * bPrev * bPrev;          /* bPrev³ */

        seq_t inner = a3 + (seq_t)4 * b3;          /* a³ + 4*b³ */

        seq_t x = aPrev * inner;                   /* numerator before reduction */

        seq_t y = (seq_t)2 * bPrev * (a3 + b3);    /* denominator before reduction */

        seq_t g = gcd(x, y);
        num_array[i] = x / g;
        den_array[i] = y / g;
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    ensure_computed(n);
    return num_array[n];
}
