/*
 * A251647  Number of (n+2)X(3+2) 0..3 arrays with no row, column, diagonal or
 *          antidiagonal in any 3X3 subblock summing to 0 2 4 5 7 or 9.
 * https://oeis.org/A251647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 17, u64 n <= 41, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A251647: Number of (n+2)×5 arrays with elements 0..3 in which
 * no row, column, diagonal or antidiagonal in any 3×3 subblock sums
 * to 0,2,4,5,7 or 9.
 *
 * Empirical linear recurrence (order 9):
 *   a(n) = 4*a(n-1) - 2*a(n-2) +10*a(n-3) -60*a(n-4) +34*a(n-5)
 *          +95*a(n-6) -64*a(n-7) -32*a(n-8) +16*a(n-9)   for n>13.
 * Initial terms (1‑based):
 *   a(1)=738   a(2)=1928   a(3)=4522   a(4)=9918   a(5)=31022   a(6)=74194
 *   a(7)=163318 a(8)=509054 a(9)=1221194 a(10)=2704574 a(11)=8386814 a(12)=20177362
 *   a(13)=44950790
 *
 * The implementation memoises computed values so calls can appear in any
 * order.  All arithmetic is performed using seq_t operands only, avoiding
 * intermediate overflow for the 64‑bit build and allowing arbitrary‑precision
 * behaviour when seq_t is a big‑integer class.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms (index 0 unused). */
    static const seq_t base_vals[14] = {
        0ULL,          /* unused */
        738ULL, 1928ULL, 4522ULL, 9918ULL, 31022ULL, 74194ULL,
        163318ULL, 509054ULL, 1221194ULL, 2704574ULL,
        8386814ULL, 20177362ULL, 44950790ULL
    };

    /* undefined for n==0, return 0 */
    if (n == 0ULL)
        return 0;

    /* values 1..13 are known */
    if (n <= 13ULL)
        return base_vals[n];

    /* static memoisation buffer */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0ULL;   /* largest n already computed */

    /* initial allocation and copy known terms */
    if (cache == NULL) {
        cache_max = 13ULL;
        cache = (seq_t *)malloc((cache_max + 1ULL) * sizeof(seq_t));
        if (cache == NULL)
            abort();                       /* allocation failure */
        for (idx_t i = 1ULL; i <= 13ULL; ++i)
            cache[i] = base_vals[i];
    }

    /* extend cache if necessary */
    if (n > cache_max) {
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (n + 1ULL) * sizeof(seq_t));
        if (new_cache == NULL)
            abort();
        cache = new_cache;

        /* compute missing terms using the recurrence */
        for (idx_t i = cache_max + 1ULL; i <= n; ++i) {
            seq_t val = 4 * cache[i - 1ULL];
            val -= 2 * cache[i - 2ULL];
            val += 10 * cache[i - 3ULL];
            val -= 60 * cache[i - 4ULL];
            val += 34 * cache[i - 5ULL];
            val += 95 * cache[i - 6ULL];
            val -= 64 * cache[i - 7ULL];
            val -= 32 * cache[i - 8ULL];
            val += 16 * cache[i - 9ULL];
            cache[i] = val;
        }
        cache_max = n;
    }

    return cache[n];
}
