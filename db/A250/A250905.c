/*
 * A250905  Number of (7+1) X (n+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 13, u64 n <= 35, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Empirical Linear Recurrence for A250905:
   a(1) = 40357
   a(2) = 144442
   a(3) = 412006
   a(4) = 1052787
   a(5) = 2553537
   a(6) = 6060594
   a(7) = 14283986
   a(8) = 33622407
   a(9) = 79146865
   For n ≥ 10:
   a(n) = 14*a(n‑1) - 85*a(n‑2) + 294*a(n‑3) - 639*a(n‑4) + 906*a(n‑5)
          - 839*a(n‑6) + 490*a(n‑7) - 164*a(n‑8) + 24*a(n‑9);
*/

#include <stddef.h>
#include <stdlib.h>

static void grow_cache(seq_t **arr, size_t *cap, size_t need)
{
    size_t newcap = *cap ? *cap : 32;
    while (newcap < need) newcap <<= 1;
    *arr = (seq_t*)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t last_n = 0;   /* largest n whose value is already stored */

    if (n < 1) return 0;        /* undefined for n<1 */

    /* initialise first nine terms on first call */
    if (cache == NULL) {
        grow_cache(&cache, &cap, 10);   /* allocate at least indices 0..9 */

        static const seq_t init[9] = {
            40357,    144442,   412006,
            1052787,  2553537,  6060594,
            14283986, 33622407, 79146865
        };
        for (size_t i = 1; i <= 9; ++i) {
            cache[i] = init[i-1];
        }
        last_n = 9;
    }

    if ((size_t)n <= last_n) return cache[n];

    /* grow the cache to accommodate up to n */
    grow_cache(&cache, &cap, n + 1);

    /* compute terms from last_n + 1 up to n using the recurrence */
    for (size_t i = last_n + 1; i <= n; ++i) {
        seq_t val = 14 * cache[i-1];
        val += (-85) * cache[i-2];
        val += 294 * cache[i-3];
        val += (-639) * cache[i-4];
        val += 906 * cache[i-5];
        val += (-839) * cache[i-6];
        val += 490 * cache[i-7];
        val += (-164) * cache[i-8];
        val += 24 * cache[i-9];
        cache[i] = val;
    }
    last_n = n;
    return cache[n];
}
