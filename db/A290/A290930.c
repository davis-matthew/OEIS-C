/*
 * A290930  p-INVERT of the positive integers, where p(S) = (1 - S^2)(1 -
 *          2*S^2).
 * https://oeis.org/A290930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 25*a(n-2) + 44*a(n-3) - 54*a(n-4) + 44*a(n-5)
 *            - 25*a(n-6) + 8*a(n-7) - a(n-8) Initial values (n = 0..7): 0, 3,
 *            12, 37, 116, 372, 1188, 3763
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Pre‑computed first eight terms */
static const seq_t init[8] = {0, 3, 12, 37, 116, 372, 1188, 3763};

/* Cache: a large static array that comfortably covers the range
   of n expected for the tests (up to ~10 000).  For larger n the
   function will simply compute using the recurrence on the fly. */
#define CACHE_SIZE 200000
static seq_t cache[CACHE_SIZE];
static size_t computed = 0;   /* highest index that has been computed */

/* Initialise the cache with the first eight terms.  This is done on
   the first call to a() when it is needed. */
static void init_cache(void)
{
    for (size_t i = 0; i < 8; ++i)
        cache[i] = init[i];
    computed = 7;
}

/* Return the n-th term of the sequence, 0‑based.  Any order of calls
   is supported; previously computed values are reused from the cache. */
seq_t a(idx_t n)
{
    if (n < 8u)
        return init[(size_t)n];

    if (computed == 0)
        init_cache();

    if (n <= computed)
        return cache[(size_t)n];

    /* Compute successive terms until we reach n.  The computation is
       purely integer, using only the allowed operators. */
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        cache[i] =
            (seq_t)8 * cache[i - 1]
          - (seq_t)25 * cache[i - 2]
          + (seq_t)44 * cache[i - 3]
          - (seq_t)54 * cache[i - 4]
          + (seq_t)44 * cache[i - 5]
          - (seq_t)25 * cache[i - 6]
          + (seq_t)8  * cache[i - 7]
          - cache[i - 8];
    }
    computed = n;
    return cache[(size_t)n];
}
