/*
 * A318749  Number of pairwise relatively nonprime strict factorizations of n
 *          (no two factors are coprime).
 * https://oeis.org/A318749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A318749: number of pairwise relatively nonprime strict factorizations of n
   (strict means factors are >1 and strictly increasing; every pair shares a
   common prime factor).  The function enumerates all such factorizations. */

#include <stdint.h>
#include <stdbool.h>

/* Euclidean GCD on uint64_t */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Maximum depth of recursion is < 20 for n <= 10000 */
static uint64_t cur_factors[20];
static int cur_len = 0;

/* Recursive DFS enumerating factorisations */
static void dfs_uint64(uint64_t rem, uint64_t min_fac, seq_t *cnt)
{
    /* Count the factorisation that ends with rem, if rem >= min_fac */
    if (rem >= min_fac) {
        bool good = true;
        for (int i = 0; i < cur_len; ++i) {
            if (gcd_uint64(cur_factors[i], rem) == 1) {
                good = false;
                break;
            }
        }
        if (good)
            (*cnt)++;
    }

    /* Try to split rem further */
    for (uint64_t f = min_fac; f * f <= rem; ++f) {
        if (rem % f != 0)
            continue;
        /* Does f share a factor >1 with all previously chosen factors? */
        bool ok = true;
        for (int i = 0; i < cur_len; ++i) {
            if (gcd_uint64(cur_factors[i], f) == 1) {
                ok = false;
                break;
            }
        }
        if (!ok)
            continue;
        /* Add f and recurse */
        cur_factors[cur_len++] = f;
        dfs_uint64(rem / f, f + 1, cnt);
        cur_len--;
    }
}

/* Main entry function */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;          /* by definition a(1)=1 */

    seq_t count = 0;
    cur_len = 0;
    dfs_uint64((uint64_t)n, 2, &count);
    return count;
}
