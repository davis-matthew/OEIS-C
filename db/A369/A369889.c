/*
 * A369889  The sum of squarefree divisors of the cubefree numbers.
 * https://oeis.org/A369889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A048250(A004709(n))
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   squarefree_divisor_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/squarefree_divisor_sum.h"

/* ---------- state --------- */
static uint64_t *cubefree_vals = NULL; /* cubefree integers as uint64_t */
static size_t cubefree_len = 0;        /* number of stored cubefree values   */
static uint64_t next_check = 1;        /* next integer to test             */

/* ---------- helpers ---------- */

/* test whether x has a prime factor of exponent ≥ 3 (not cubefree) */
static bool is_cubefree(uint64_t x)
{
    for (uint64_t p = 2; p * p <= x; ++p)
    {
        int cnt = 0;
        while (x % p == 0)
        {
            ++cnt;
            x /= p;
            if (cnt >= 3)
                return false;
        }
    }
    return true;
}

/* enlarge the storage until it holds at least n cubefree numbers */
static void ensure_cubofree(idx_t n)
{
    if (cubefree_len >= n)
        return;

    size_t new_cap = cubefree_len == 0 ? 16 : cubefree_len * 2;
    while (new_cap < n)
        new_cap *= 2;

    uint64_t *tmp = (uint64_t *)realloc(cubefree_vals, new_cap * sizeof(uint64_t));
    if (tmp == NULL)
        abort();                  /* out of memory */
    cubefree_vals = tmp;

    while (cubefree_len < n)
    {
        if (is_cubefree(next_check))
            cubefree_vals[cubefree_len++] = next_check;
        ++next_check;
    }
}

/* ---------- public sequence function ---------- */
extern seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n = 0, guard anyway */
        return 0;
    ensure_cubofree(n);
    uint64_t m = cubefree_vals[n - 1];
    return squarefree_divisor_sum(m);
}
