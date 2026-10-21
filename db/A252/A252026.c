/*
 * A252026  Number of (n+2)X(2+2) 0..2 arrays with every 3X3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 0 2 or 4.
 * https://oeis.org/A252026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 18, u64 n <= 39, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A252026
   The sequence satisfies the linear recurrence
   a(n) = 3a(n‑1) – a(n‑2) + 6a(n‑3) –12a(n‑4) –3a(n‑5)
          – a(n‑6) +6a(n‑7) + a(n‑8) + 6a(n‑9) + a(n‑10)
          +3a(n‑11) –3a(n‑12) –3a(n‑13) –2a(n‑14)    for n > 18
   with initial values
      a(1)=227, a(2)=323, a(3)=781, a(4)=1929,
      a(5)=5285, a(6)=14831, a(7)=41745, a(8)=117867,
      a(9)=332997, a(10)=941237, a(11)=2660915,
      a(12)=7523177, a(13)=21271317, a(14)=60144341,
      a(15)=170058799, a(16)=480846469,
      a(17)=1359611219, a(18)=3844354431.
   The function computes a(n) lazily, caching all computed
   values so that any order of calls is supported.
*/

#include <stdlib.h>

static const seq_t init[19] = {
    0ULL,               /* dummy entry for index 0 */
    227ULL,
    323ULL,
    781ULL,
    1929ULL,
    5285ULL,
    14831ULL,
    41745ULL,
    117867ULL,
    332997ULL,
    941237ULL,
    2660915ULL,
    7523177ULL,
    21271317ULL,
    60144341ULL,
    170058799ULL,
    480846469ULL,
    1359611219ULL,
    3844354431ULL
};

seq_t a(idx_t n)
{
    /* Return early for invalid indices. */
    if (n == 0)
        return 0;

    /* For the first 18 terms, use the constants. */
    if (n <= 18)
        return init[n];

    /* Lazy caching of previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t computed_up_to = 18;

    /* Allocate initial cache on first call. */
    if (cache == NULL) {
        cache_len = 19;                      /* indices 0..18 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL)
            return 0;                       /* allocation failure */
        for (idx_t i = 1; i <= 18; ++i)
            cache[i] = init[i];
        computed_up_to = 18;
    }

    /* Grow the cache if the requested index is beyond the allocated length. */
    if (n >= cache_len) {
        idx_t new_len = n + 1;               /* we need indices up to n */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL)
            return 0;                       /* allocation failure */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute missing values from computed_up_to+1 up to n. */
    if (n > computed_up_to) {
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            seq_t pos = 0, neg = 0;

            /* Positive terms */
            pos += (seq_t)3 * cache[i - 1];
            pos += (seq_t)6 * cache[i - 3];
            pos += (seq_t)6 * cache[i - 7];
            pos += (seq_t)6 * cache[i - 9];
            pos += (seq_t)3 * cache[i - 11];
            pos += cache[i - 8];
            pos += cache[i - 10];

            /* Negative terms */
            neg += cache[i - 2];
            neg += (seq_t)12 * cache[i - 4];
            neg += (seq_t)3 * cache[i - 5];
            neg += cache[i - 6];
            neg += (seq_t)3 * cache[i - 12];
            neg += (seq_t)3 * cache[i - 13];
            neg += (seq_t)2 * cache[i - 14];

            cache[i] = pos - neg;            /* pos ≥ neg, so result is non‑negative */
        }
        computed_up_to = n;
    }

    return cache[n];
}
