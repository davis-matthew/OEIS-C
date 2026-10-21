/*
 * A243030  Number of length n+3 0..5 arrays with no four elements in a row
 *          with pattern abba (possibly a=b) and new values 0..5 introduced in
 *          0..5 order.
 * https://oeis.org/A243030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) -34*a(n-2) +23*a(n-3) -25*a(n-4) +127*a(n-5)
 *            +11*a(n-6) +50*a(n-7) -155*a(n-8) -124*a(n-9) -158*a(n-10)
 *            -59*a(n-11) -30*a(n-12) (n > 12) Initial values: a(1)=13,
 *            a(2)=43, a(3)=163, a(4)=693, a(5)=3223, a(6)=16060, a(7)=84207,
 *            a(8)=457676, a(9)=2549156, a(10)=14430921, a(11)=82566...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* coefficients of the 12‑term linear recurrence */
static const int coef[12] = {
    11, -34, 23, -25, 127, 11, 50, -155, -124, -158, -59, -30
};

/* helper arrays for computing the first 12 base terms by definition */
#define SENTINEL 6  /* value > 5 to denote “not yet set” in the last 3 slots */
#define MAX_POS 15  /* maximum length for n <= 12 → L = n+3 <= 15 */
static unsigned char visited[MAX_POS + 1][6][7][7][7];
static seq_t memo[MAX_POS + 1][6][7][7][7];

/* recursive DP that counts legal arrays of length L (L = n+3) */
static seq_t dfs(int pos, int max_val, int l1, int l2, int l3, int L)
{
    if (pos == L)
        return (seq_t)1;

    if (visited[pos][max_val][l1][l2][l3])
        return memo[pos][max_val][l1][l2][l3];

    seq_t total = 0;

    /* choose an existing value 0 .. max_val */
    for (int v = 0; v <= max_val; ++v) {
        if (pos >= 3 && l1 == v && l2 == l3)
            continue;                   /* would create abba */
        total += dfs(pos + 1, max_val,
                     l2, l3, v, L);
    }

    /* choose a new value max_val+1 if allowed (max 5) */
    if (max_val < 5) {
        int v = max_val + 1;
        if (!(pos >= 3 && l1 == v && l2 == l3))
            total += dfs(pos + 1, max_val + 1,
                         l2, l3, v, L);
    }

    visited[pos][max_val][l1][l2][l3] = 1;
    memo[pos][max_val][l1][l2][l3] = total;
    return total;
}

/* counts a(n) for n <= 12 by brute force definition */
static seq_t compute_small(idx_t n)
{
    int L = (int)n + 3;               /* length of the array */
    /* clear DP tables */
    for (size_t i = 0; i < sizeof(visited); ++i)
        ((unsigned char *)&visited)[i] = 0;

    /* first element must be 0 (introducing the first value) */
    return dfs(1, 0, SENTINEL, SENTINEL, 0, L);
}

/* cached base values a(1)..a(12) */
static seq_t base_values[12];
static int base_computed = 0;

/* main function requested by the problem statement */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* compute base values on first use */
    if (!base_computed) {
        for (int i = 1; i <= 12; ++i)
            base_values[i - 1] = compute_small((idx_t)i);
        base_computed = 1;
    }

    if (n <= 12)
        return base_values[(idx_t)n - 1];

    /* memoised recurrence for n > 12 */
    static seq_t *cache = NULL;   /* cache[0] holds a(1) */
    static idx_t cache_n = 0;     /* number of values currently stored */

    /* allocate or enlarge cache as needed */
    if (cache == NULL) {
        cache = (seq_t *)malloc(12 * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;
        for (idx_t i = 0; i < 12; ++i)
            cache[i] = base_values[(size_t)i];
        cache_n = 12;
    }

    if (cache_n < n) {
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (new_cache == NULL)
            return (seq_t)0;
        cache = new_cache;

        /* compute new terms using the linear recurrence */
        for (idx_t i = cache_n; i < n; ++i) {
            seq_t sum = 0;
            for (int k = 0; k < 12; ++k) {
                int c = coef[k];
                if (c >= 0)
                    sum += (seq_t) c * cache[(size_t)(i - 1 - k)];
                else
                    sum -= (seq_t)(-c) * cache[(size_t)(i - 1 - k)];
            }
            cache[i] = sum;
        }
        cache_n = n;
    }

    return cache[(size_t)n - 1];
}
