/*
 * A262789  Number of (n+2) X (2+2) 0..1 arrays with each row divisible by 5
 *          and column not divisible by 5, read as a binary number with top
 *          and left being the most significant bits.
 * https://oeis.org/A262789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=36, a(2)=144, a(3)=625, a(4)=2601, a(5)=10404, a(6)=41616,
 * a(7)=167281, a(8)=670761;
 * for n≥9:  a(n)=6*a(n-1)-12*a(n-2)+24*a(n-3)-31*a(n-4)-6*a(n-5)
 *            +12*a(n-6)-24*a(n-7)+32*a(n-8).   */
#include <stdlib.h>

/* The function returns the n‑th term of OEIS A262789.
   It memoises computed terms in a dynamically resized array.
   The array is indexed 1‑based; index 0 is unused but kept to simplify
   bounds checks.  The function allows calls in any order.  Memory is
   allocated with malloc/realloc and freed automatically by the
   program termination. */
seq_t a(idx_t n)
{
    /* Pre‑defined initial values for n = 1 … 8 (index 0 unused). */
    static const seq_t init[9] = {
        0ULL,
        (seq_t)36ULL,
        (seq_t)144ULL,
        (seq_t)625ULL,
        (seq_t)2601ULL,
        (seq_t)10404ULL,
        (seq_t)41616ULL,
        (seq_t)167281ULL,
        (seq_t)670761ULL
    };

    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated size (number of elements) */
    static idx_t last_computed = 0; /* highest n that has been computed */

    if (n == 0)          /* below the offset – undefined, return 0 */
        return (seq_t)0;

    /* Ensure the array is large enough for indices 0 … n. */
    size_t needed = (size_t)n + 1;
    if (!cache) {
        cache = (seq_t*)malloc(needed * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;  /* allocation failed – return 0 */
        cache_size = needed;
    } else if (needed > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;  /* allocation failed – return 0 */
        cache = tmp;
        cache_size = needed;
    }

    /* Compute any missing values up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 8) {
            cache[i] = init[i];
        } else {
            /* Apply the 8‑term linear recurrence. */
            seq_t t1 = (seq_t)6 *  cache[i - 1];
            seq_t t2 = (seq_t)12 * cache[i - 2];
            seq_t t3 = (seq_t)24 * cache[i - 3];
            seq_t t4 = (seq_t)31 * cache[i - 4];
            seq_t t5 = (seq_t)6  * cache[i - 5];
            seq_t t6 = (seq_t)12 * cache[i - 6];
            seq_t t7 = (seq_t)24 * cache[i - 7];
            seq_t t8 = (seq_t)32 * cache[i - 8];
            cache[i] = t1 - t2 + t3 - t4 - t5 + t6 - t7 + t8;
        }
    }

    if (n > last_computed)
        last_computed = n;

    return cache[n];
}
