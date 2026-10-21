/*
 * A254990  4-bonacci word. Fixed point of morphism 0->01, 1->02, 2->03, 3->0.
 * https://oeis.org/A254990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * 4‑bonacci word – the fixed point of the morphism
 *   0 → 01 , 1 → 02 , 2 → 03 , 3 → 0
 *
 *  a(0) = 0
 *  Recurrence for the iterate S(n):
 *    S(0) = 0
 *    S(1) = 01
 *    S(2) = 0102
 *    S(3) = 01020103
 *    for n ≥ 4: S(n) = S(n‑1) S(n‑2) S(n‑3) S(n‑4)
 *
 *  The sequence a(n) is the infinite word S(∞).  We compute a(n)
 *  by keeping the lengths of the iterates and descending recursively
 *  to one of the base words S(0)…S(3).  This method works for any
 *  seq_t and idx_t as long as seq_t can hold a uint8_t.
 */

#include <stdint.h>
#include <stdlib.h>

/* base words S(0)…S(3), 0‑based indexing */
static const uint8_t small_word[4][8] = {
    {0},                               /* S(0) */
    {0, 1},                            /* S(1) */
    {0, 1, 0, 2},                      /* S(2) */
    {0, 1, 0, 2, 0, 1, 0, 3}            /* S(3) */
};

/* cache of lengths of S(i) (uint64_t as lengths never exceed 2^64-1) */
static uint64_t *len_cache = NULL;   /* dynamic array of lengths */
static size_t   len_used   = 0;       /* number of stored lengths */
static size_t   len_cap    = 0;       /* capacity of len_cache */

/* make sure len_cache holds lengths for all i with len_cache[i] > n */
static void
ensure_lengths(uint64_t n)
{
    /* initialise on first call */
    if (len_cache == NULL) {
        len_cap = 8;
        len_cache = (uint64_t *)malloc(len_cap * sizeof(uint64_t));
        len_cache[0] = 1ULL;   /* length of S(0) */
        len_cache[1] = 2ULL;   /* length of S(1) */
        len_cache[2] = 4ULL;   /* length of S(2) */
        len_cache[3] = 8ULL;   /* length of S(3) */
        len_used = 4;
    }

    /* grow capacity if needed */
    if (len_cap < len_used + 4) {
        len_cap = len_cap * 2;
        len_cache = (uint64_t *)realloc(len_cache, len_cap * sizeof(uint64_t));
    }

    /* generate further lengths until we exceed n */
    while (len_cache[len_used - 1] <= n) {
        unsigned __int128 sum =
            (unsigned __int128)len_cache[len_used - 1] +
            (unsigned __int128)len_cache[len_used - 2] +
            (unsigned __int128)len_cache[len_used - 3] +
            (unsigned __int128)len_cache[len_used - 4];
        if (sum > (unsigned __int128)UINT64_MAX)
            len_cache[len_used] = UINT64_MAX;
        else
            len_cache[len_used] = (uint64_t)sum;
        len_used++;
    }
}

seq_t
a(idx_t n)
{
    /* ensure we have enough length information */
    ensure_lengths((uint64_t)n);

    /* find the smallest k such that len_cache[k] > n */
    size_t k = 0;
    while (k + 1 < len_used && len_cache[k] <= (uint64_t)n) {
        k++;
    }

    /* descend recursively to the base word */
    uint64_t pos = (uint64_t)n;
    while (k > 3) {
        uint64_t l1 = len_cache[k - 1];
        uint64_t l2 = len_cache[k - 2];
        uint64_t l3 = len_cache[k - 3];
        /* l4 = len_cache[k - 4]; implicit in next else */

        if (pos < l1) {
            k--;                     /* inside S(k-1) part */
        } else if (pos < l1 + l2) {
            pos -= l1;
            k -= 2;                 /* inside S(k-2) part */
        } else if (pos < l1 + l2 + l3) {
            pos -= l1 + l2;
            k -= 3;                 /* inside S(k-3) part */
        } else {
            pos -= l1 + l2 + l3;
            k -= 4;                 /* inside S(k-4) part */
        }
    }

    /* k is 0,1,2,or 3 – look up the pre‑computed symbol */
    return (seq_t)small_word[k][(size_t)pos];
}
