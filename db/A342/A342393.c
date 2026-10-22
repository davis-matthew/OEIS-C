/*
 * A342393  Cubefree numbers a > 1 such that the ring of integers of
 *          Q(a^(1/3)) is not Z[a^(1/3)].
 * https://oeis.org/A342393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 9859, u32 n <= 9859, u64 n <= 9859,
 *            u128 n <= 9859, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A342393: a(n) is the nth cubefree integer a>1 such that
 * the ring of integers of Q(a^(1/3)) is not Z[a^(1/3)].
 * An integer a satisfies this iff it is cubefree and
 * (a is NOT squarefree OR a ≡ 1 or 8 (mod 9)).
 * The algorithm generates candidates in increasing order and
 * keeps a static cache of the first n terms that have been found.
 * The cache is grown with realloc; for an arbitrary-precision
 * build `seq_t` is a class, so the pointer type is cast explicitly.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Test whether n is cubefree and satisfies the condition. */
static bool satisfies(uint64_t n)
{
    uint64_t x = n;
    bool is_sq = true;          /* true iff no squared prime factor yet */
    for (uint64_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            int exp = 0;
            while (x % p == 0)
            {
                x /= p;
                ++exp;
                if (exp >= 3)   /* a^3 divides n → not cubefree */
                    return false;
            }
            if (exp >= 2)
                is_sq = false;     /* found a square factor */
        }
    }
    /* x > 1 now holds a remaining prime factor with exponent 1 */
    if (!is_sq)                /* not squarefree */
        return true;
    uint64_t mod9 = n % 9;
    return (mod9 == 1 || mod9 == 8);
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* undefined for n=0, harmless fallback */

    /* Static cache of previously found terms */
    static seq_t *cache = NULL;
    static size_t len   = 0;   /* number of terms stored */
    static size_t cap   = 0;   /* capacity of cache array */
    static uint64_t cand = 1;  /* last candidate tested */

    /* Generate terms until we have at least n of them */
    while (len < n)
    {
        ++cand;                   /* start with 2 on first iteration */
        if (cand < 2)             /* skip 1 */
            continue;
        if (satisfies(cand))
        {
            /* Grow cache if necessary */
            if (len == cap)
            {
                size_t newcap = cap ? cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();     /* unlikely but safe */
                cache = tmp;
                cap   = newcap;
            }
            cache[len++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
