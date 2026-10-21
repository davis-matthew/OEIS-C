/*
 * A260065  Number of (n+2)X(3+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000001 00000011 or 00000111.
 * https://oeis.org/A260065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 21, u64 n <= 49, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence (OEIS A260065):
   a(n) = 7*a(n-3) + 15*a(n-4) - 8*a(n-6) - 29*a(n-7) - 36*a(n-8)
          - 3*a(n-9) - 10*a(n-10) + 4*a(n-11) + 26*a(n-12)
          - 23*a(n-13) - 12*a(n-14) - 28*a(n-15) - 8*a(n-17)   (n > 21)
   Initial values (for 1 ≤ n ≤ 21):
   a(1)=256, a(2)=594, a(3)=1680, a(4)=3825, a(5)=6714,
   a(6)=16344, a(7)=42094, a(8)=84761, a(9)=173654,
   a(10)=438404, a(11)=996340, a(12)=2016015, a(13)=4598146,
   a(14)=11017398, a(15)=23615180, a(16)=50616069,
   a(17)=118597002, a(18)=268738044, a(19)=575235736,
   a(20)=1289517999, a(21)=2973476636 */
#include <stdlib.h>

static const seq_t INITIAL[22] = {
    0ULL,           /* dummy for index 0 */
    256ULL, 594ULL, 1680ULL, 3825ULL, 6714ULL,
    16344ULL, 42094ULL, 84761ULL, 173654ULL,
    438404ULL, 996340ULL, 2016015ULL, 4598146ULL,
    11017398ULL, 23615180ULL, 50616069ULL,
    118597002ULL, 268738044ULL, 575235736ULL,
    1289517999ULL, 2973476636ULL
};

seq_t a(idx_t n)
{
    if (n <= 21U)
        return INITIAL[(idx_t)n];

    /* static cache, initialized on first call */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;    /* capacity in elements (≥1) */
    static idx_t last = 0;        /* highest index computed, 0 = not yet */

    /* allocate cache once and copy known initial values */
    if (last == 0) {
        capacity = 64;                      /* initial arbitrary size */
        cache = (seq_t*)calloc(capacity, sizeof(seq_t));
        if (!cache)
            return 0;                       /* allocation failure – unlikely */

        for (idx_t i = 1; i <= 21U; ++i)
            cache[i] = INITIAL[i];
        last = 21U;
    }

    /* grow the cache as necessary */
    while (capacity <= n) {
        size_t next = (size_t)capacity * 2U;
        if (next <= capacity)                 /* protect against overflow */
            next = capacity + 1U;
        seq_t *tmp = (seq_t*)realloc(cache, next * sizeof(seq_t));
        if (!tmp)
            return 0;                         /* allocation failure */
        cache = tmp;
        capacity = (idx_t)next;
    }

    /* compute all missing terms up to n */
    while (last < n) {
        idx_t i = last + 1U;
        seq_t val = 0;

        val += (seq_t)7  * cache[i - 3U];
        val += (seq_t)15 * cache[i - 4U];
        val -= (seq_t)8  * cache[i - 6U];
        val -= (seq_t)29 * cache[i - 7U];
        val -= (seq_t)36 * cache[i - 8U];
        val -= (seq_t)3  * cache[i - 9U];
        val -= (seq_t)10 * cache[i - 10U];
        val += (seq_t)4  * cache[i - 11U];
        val += (seq_t)26 * cache[i - 12U];
        val -= (seq_t)23 * cache[i - 13U];
        val -= (seq_t)12 * cache[i - 14U];
        val -= (seq_t)28 * cache[i - 15U];
        val -= (seq_t)8  * cache[i - 17U];

        cache[i] = val;
        ++last;
    }

    return cache[n];
}
