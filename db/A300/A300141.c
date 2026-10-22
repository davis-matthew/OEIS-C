/*
 * A300141  Number of nX3 0..1 arrays with every element equal to 1, 3, 4, 5,
 *          6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A300141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A300141: number of n×3 binary arrays with upper-left corner 0
 * and kings adjacent cells equal to 1,3,4,5,6,7 or 8.
 *
 * Recurrent formula (OEIS):
 *   a(n) = 2*a(n-1) + 3*a(n-2) + 18*a(n-3) - 21*a(n-4) - 54*a(n-5) - 54*a(n-6)
 *          + 79*a(n-7) + 119*a(n-8) + 4*a(n-9) - 3*a(n-10) - a(n-11)
 *          + 26*a(n-12) - 53*a(n-13) - 17*a(n-14) + 15*a(n-15)
 *   for n > 17, with initial values:
 *   a(1)=0, a(2)=4, a(3)=1, a(4)=18, a(5)=41, a(6)=130,
 *   a(7)=510, a(8)=1740, a(9)=5554, a(10)=20089, a(11)=69607,
 *   a(12)=233844, a(13)=815086, a(14)=2824883, a(15)=9656170,
 *   a(16)=33351986, a(17)=115270625.
 */

#include <stdlib.h>
#include <stdint.h>

/* Basic geometric series as an alternative formula, keep comments. */

/* Base values 1…17 */
static const seq_t base_values[17] = {
    0ULL,  /* a(1) */
    4ULL,  /* a(2) */
    1ULL,  /* a(3) */
    18ULL, /* a(4) */
    41ULL, /* a(5) */
    130ULL,/* a(6) */
    510ULL,/* a(7) */
    1740ULL,/* a(8) */
    5554ULL,/* a(9) */
    20089ULL,/* a(10) */
    69607ULL,/* a(11) */
    233844ULL,/* a(12) */
    815086ULL,/* a(13) */
    2824883ULL,/* a(14) */
    9656170ULL,/* a(15) */
    33351986ULL,/* a(16) */
    115270625ULL/* a(17) */
};

static seq_t *cache = NULL;     /* 1‑based array of computed terms */
static size_t cache_len = 0;    /* current allocated length (including slot 0) */

/* Ensure that cache holds values up to index n (1‑based). */
static void ensure_cache(idx_t n)
{
    /* size_t may be smaller than idx_t on a 32‑bit build; all test
       values fit into the accessible address space. */
    size_t needed = (size_t)n + 1;          /* index 0 unused */
    if (cache_len > 0 && needed <= cache_len)
        return;                            /* already sufficient */

    seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (new_cache == NULL)
        abort();                            /* out of memory */

    /* Initialise new entries to zero.  The slot 0 stays unused. */
    for (size_t i = cache_len; i < needed; ++i)
        new_cache[i] = 0;

    /* Compute new terms.  cache_len is the first new index. */
    for (size_t i = cache_len; i < needed; ++i)
    {
        if (i == 0)
            continue;                       /* unused */

        if (i <= 17)
        {
            /* Predefined initial values (1‑based indexing). */
            new_cache[i] = base_values[i - 1];
            continue;
        }

        /* Recurrence computation for i > 17.  All operations use seq_t
           arithmetic only, complying with the mandatory operator set. */
        seq_t pos = 0;
        pos += new_cache[i - 1] * 2;
        pos += new_cache[i - 2] * 3;
        pos += new_cache[i - 3] * 18;
        pos += new_cache[i - 7] * 79;
        pos += new_cache[i - 8] * 119;
        pos += new_cache[i - 9] * 4;
        pos += new_cache[i - 12] * 26;
        pos += new_cache[i - 15] * 15;

        seq_t neg = 0;
        neg += new_cache[i - 4] * 21;
        neg += new_cache[i - 5] * 54;
        neg += new_cache[i - 6] * 54;
        neg += new_cache[i - 10] * 3;
        neg += new_cache[i - 11] * 1;
        neg += new_cache[i - 13] * 53;
        neg += new_cache[i - 14] * 17;

        new_cache[i] = pos - neg;
    }

    cache = new_cache;
    cache_len = needed;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* offset starts at 1, behaviour below not required */
    ensure_cache(n);
    return cache[(size_t)n];
}
