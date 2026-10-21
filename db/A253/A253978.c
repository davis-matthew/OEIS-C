/*
 * A253978  Number of (n+2)X(1+2) 0..1 arrays with every 3X3 subblock diagonal
 *          maximum plus antidiagonal maximum nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A253978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A253978
 *  Empirical recurrence (empirical):
 *  a(n) = 7*a(n-1) - 6*a(n-2) + 18*a(n-3) - 3*a(n-4) - 123*a(n-5)
 *         + 14*a(n-6) - 74*a(n-7) + 60*a(n-8) + 396*a(n-9)
 *         + 120*a(n-10) + 24*a(n-11) + 32*a(n-12) - 32*a(n-13)
 *  Initial values:
 *    a(1)=512, a(2)=3372, a(3)=21304, a(4)=136868, a(5)=881432,
 *    a(6)=5659548, a(7)=36317408, a(8)=233116252, a(9)=1496407920,
 *    a(10)=9605229252, a(11)=61654061624, a(12)=395747083364,
 *    a(13)=2540236192136
 *
 *  This implementation memoises all values up to the largest n
 *  requested.  Calls may be made in any order.
 */

#include <stdlib.h>   /* for malloc/realloc */

static const seq_t init_vals[14] = {
    /* 0 unused */ 0ULL,
    (seq_t)512ULL,
    (seq_t)3372ULL,
    (seq_t)21304ULL,
    (seq_t)136868ULL,
    (seq_t)881432ULL,
    (seq_t)5659548ULL,
    (seq_t)36317408ULL,
    (seq_t)233116252ULL,
    (seq_t)1496407920ULL,
    (seq_t)9605229252ULL,
    (seq_t)61654061624ULL,
    (seq_t)395747083364ULL,
    (seq_t)2540236192136ULL
};

static seq_t *cache = NULL;  /* memoisation array */
static size_t  cap   = 0;    /* number of elements allocated (index 0 .. cap-1) */
static idx_t   lastc = 0;    /* highest index whose value is valid */

/*  Return the value a(n) for the OEIS sequence A253978.  */
seq_t a(idx_t n)
{
    if (n < 1)          /* out of range, return 0 */
        return (seq_t)0;

    /* ---------- ensure the cache is large enough ---------- */
    size_t needed = (size_t)n + 1;          /* indices 0 .. n needed */
    if (needed < 14)                       /* we always need space for init */
        needed = 14;

    if (cap < needed) {
        /* allocate new space, keep existing contents (copy anyway) */
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)                       /* unlikely, but handle */
            return (seq_t)0;
        cache = tmp;
        /* zero the newly allocated part if any */
        if (cap < needed) {
            for (size_t i = cap; i < needed; ++i)
                cache[i] = (seq_t)0;
        }
        cap = needed;
    }

    /* ---------- initialise the first 13 terms if not yet done ---------- */
    if (lastc < 13) {
        for (idx_t i = 1; i <= 13; ++i)
            cache[i] = init_vals[i];
        lastc = 13;
    }

    /* ---------- compute any terms beyond what we have ---------- */
    if (lastc < n) {
        idx_t start = (lastc >= 13 ? lastc + 1 : 14);   /* recurrence starts at 14 */
        for (idx_t i = start; i <= n; ++i) {
            seq_t pos = 7 * cache[i - 1];
            pos += 18 * cache[i - 3];
            pos += 14 * cache[i - 6];
            pos += 60 * cache[i - 8];
            pos += 396 * cache[i - 9];
            pos += 120 * cache[i - 10];
            pos += 24 * cache[i - 11];
            pos += (seq_t)32 * cache[i - 12];

            seq_t neg = 6 * cache[i - 2];
            neg += 3 * cache[i - 4];
            neg += 123 * cache[i - 5];
            neg += 74 * cache[i - 7];
            neg += 32 * cache[i - 13];

            cache[i] = pos - neg;
        }
        lastc = n;
    }

    return cache[n];
}
