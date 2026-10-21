/*
 * A294759  Number of permutations of [n] avoiding {4321, 1243, 1234}.
 * https://oeis.org/A294759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = 0 for n ≥ 10 (by Erdős–Szekeres). For 0 ≤ n ≤ 9, a(n)
 *            equals the number of permutations of [n] that avoid the patterns
 *            4321, 1243, 1234. The values are computed by enumerating all n!
 *            permutations and testing each for the forbidden patterns
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether a permutation contains any of the forbidden patterns. */
static bool has_forbidden(const int *p, idx_t len)
{
    for (idx_t i = 0; i + 3 < len; ++i)
        for (idx_t j = i + 1; j + 2 < len; ++j)
            for (idx_t k = j + 1; k + 1 < len; ++k)
                for (idx_t l = k + 1; l < len; ++l) {
                    int a = p[i], b = p[j], c = p[k], d = p[l];
                    /* 1243: a < b < d < c */
                    if (a < b && b < d && d < c)
                        return true;
                    /* 1234: a < b < c < d */
                    if (a < b && b < c && c < d)
                        return true;
                    /* 4321: a > b > c > d */
                    if (a > b && b > c && c > d)
                        return true;
                }
    return false;
}

/* Enumerate all permutations of size n and count the acceptable ones. */
static seq_t count_acceptable(idx_t n)
{
    seq_t cnt = 0;
    int perm[9];                     /* maximum n is 9 for n < 10 */
    for (idx_t i = 0; i < n; ++i)
        perm[i] = (int)(i + 1);      /* use values 1..n */

    /* Count the initial permutation. */
    if (!has_forbidden(perm, n))
        ++cnt;

    /* Generate next permutations in lexicographic order. */
    while (true) {
        /* Find the rightmost position k with perm[k] < perm[k+1] */
        int k = (int)(n - 1);
        while (k > 0 && perm[k - 1] > perm[k]) --k;
        if (k == 0) break;           /* last permutation reached */

        /* Find the rightmost element larger than perm[k-1]. */
        int l = (int)(n - 1);
        while (perm[l] < perm[k - 1]) --l;

        /* Swap perm[k-1] and perm[l]. */
        int tmp = perm[k - 1];
        perm[k - 1] = perm[l];
        perm[l] = tmp;

        /* Reverse the suffix starting at position k. */
        for (int i = k, j = (int)n - 1; i < j; ++i, --j) {
            tmp = perm[i];
            perm[i] = perm[j];
            perm[j] = tmp;
        }

        if (!has_forbidden(perm, n))
            ++cnt;
    }
    return cnt;
}

/* Public function computing the sequence term. */
seq_t a(idx_t n)
{
    static bool computed[10] = {false};
    static seq_t cache[10] = {0};

    if (n >= 10)
        return 0;

    if (computed[n])
        return cache[n];

    if (n == 0) {
        cache[n] = 1;
    } else {
        cache[n] = count_acceptable(n);
    }
    computed[n] = true;
    return cache[n];
}
