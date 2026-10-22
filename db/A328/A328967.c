/*
 * A328967  a(n+1) = 1 - Sum_{k=1..n} a(floor(n/k)).
 * https://oeis.org/A328967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n+1) = 1 - Σ_{k=1..n} a(floor(n/k)), a(1) = 1
 * Range:     i8 n <= 64, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Index and value types are supplied externally:  idx_t is an unsigned integer
 *  (usually uint64_t), and seq_t is a signed integer type of arbitrary width
 *  (int32_t, int64_t, __int128 or an arbitrary‑precision class).  No
 *  intermediate value is allowed to exceed the natural size of seq_t. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage for memoisation (0‑based: cache[0] == a(1)) */
static seq_t *cache = NULL;   /* 0‑based array of computed values */
static size_t cap = 0;       /* allocated size of cache */
static idx_t computed = 0;   /* largest n whose value is computed (0 = none) */

/*  Compute a(n) for n ≥ 1, in O(n) time with memoisation.
 *  The function works correctly for any build where seq_t can be
 *  added, subtracted, multiplied and compared.  Calls may arrive
 *  in any order. */
seq_t a(idx_t n)
{
    if (n == 0)          /* under‑offset values are not defined */
        return 0;        /* safe default, never used for n ≥ 1 */

    /* Ensure the cache is large enough for indices 0 … n-1 */
    if ((size_t)n > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory */
        cache = tmp;
        cap = (size_t)n;
    }

    /* Compute any missing terms up to n */
    while (computed < n) {
        ++computed;                 /* now computing a(computed)  */
        idx_t v = computed - 1;      /* v = n-1 used in the recurrence   */
        seq_t sum = 0;

        /* Sum  a(⌊v / i⌋)  efficiently by grouping equal quotients */
        idx_t i = 1;
        while (i <= v) {
            idx_t q = v / i;                 /* floor(v / i)   */
            idx_t next = v / q;              /* last i with same q   */
            seq_t val = cache[q - 1];        /* a(q), 1‑based index */
            idx_t cnt = next - i + 1;        /* how many times q occurs   */
            sum += val * (seq_t)cnt;         /* weighted contribution    */
            i = next + 1;
        }

        cache[computed - 1] = (seq_t)1 - sum;  /* a(computed)            */
    }

    return cache[n - 1];
}
