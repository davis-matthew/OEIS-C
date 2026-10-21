/*
 * A293646  Sum of two (possibly negative) coprime cubes, but not the sum of 2
 *          non-coprime cubes.
 * https://oeis.org/A293646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 101, u64 n <= 101, u128 n <= 101,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th positive integer that can be expressed
 *                as the sum of two coprime integer cubes but cannot be
 *                expressed as the sum of two non‑coprime integer cubes.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* static cache of sequence values */
static seq_t *terms = NULL;    /* array of terms */
static size_t terms_len = 0;   /* number of terms in cache */
static size_t terms_cap = 0;   /* capacity of terms array */

/*
 * Ensure that at least `n` terms are cached.
 * The function recomputes the sequence from scratch when needed,
 * using a growing bound for cube roots until enough terms are found.
 */
static void ensure_terms(idx_t n)
{
    size_t target = (size_t)n;
    if (terms_len >= target)
        return;                 /* already have enough */

    idx_t cubeRoot = 64;        /* initial cube‑root bound */
    uint8_t *status = NULL;     /* classification table */

    /* find a bound large enough to contain `target` terms */
    while (true) {
        idx_t maxSum = 2 * cubeRoot * cubeRoot * cubeRoot;
        size_t st_size = (size_t)maxSum + 1;

        status = (uint8_t*)calloc(st_size, 1);
        if (!status)
            abort();

        int minX = -(int)cubeRoot;
        int maxX = (int)cubeRoot;
        for (int x = minX; x <= maxX; ++x) {
            seq_t cx = (seq_t)x * (seq_t)x * (seq_t)x;
            for (int y = minX; y <= maxX; ++y) {
                seq_t s = cx + (seq_t)y * (seq_t)y * (seq_t)y;
                if (s <= 0 || s > maxSum)
                    continue;
                seq_t ax = x >= 0 ? (seq_t)x : (seq_t)(-x);
                seq_t ay = y >= 0 ? (seq_t)y : (seq_t)(-y);
                seq_t g  = gcd(ax, ay);
                uint8_t flag = (g == 1) ? 1 : 2;
                status[(size_t)s] |= flag;
            }
        }

        /* count qualifying numbers */
        size_t cnt = 0;
        for (size_t v = 1; v <= (size_t)maxSum && cnt < target; ++v)
            if (status[v] == 1)
                ++cnt;

        if (cnt >= target)
            break;   /* sufficient bound found */

        free(status);
        cubeRoot <<= 1;       /* double the bound */
    }

    /* build the cache with exactly `target` terms */
    if (terms_cap < target) {
        free(terms);
        terms = (seq_t*)calloc(target, sizeof(seq_t));
        terms_cap = target;
    }
    size_t idx = 0;
    idx_t maxSum = 2 * cubeRoot * cubeRoot * cubeRoot;
    for (size_t v = 1; v <= (size_t)maxSum && idx < target; ++v)
        if (status[v] == 1)
            terms[idx++] = (seq_t)v;
    terms_len = idx;

    free(status);
}

/*
 * Public function required by the framework.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        abort();                /* offset starts at 1 */
    ensure_terms(n);
    return terms[n - 1];
}
