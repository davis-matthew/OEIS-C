/*
 * A386838  Minimum number of unit squares that a straight line drawn from
 *          (0,0) to (x,y) passes through on the square lattice where x^2 +
 *          y^2 = A001481(n). If A001481(n) can be written as a sum of two
 *          squares in two or more ways, x and y are chosen such that a(n) is
 *          the least value.
 * https://oeis.org/A386838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min_{x^2 + y^2 = A001481(n)} (x + y - gcd(x, y))
 * Range:     u8 n <= 8425, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (A001481 is the sequence of non‑negative integers that are a sum of two squares.) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/_floor_isqrt_u64.h"

/* ------------------------------------------------------------------ */
/* local helper: find minimal x + y - gcd(x, y) for a given m = x² + y² */
static seq_t min_f_for_m(uint64_t m, bool *found)
{
    seq_t best = (seq_t)-1;
    *found = false;

    uint64_t d = _floor_isqrt_u64(m);
    for (uint64_t x = 0; x <= d; ++x) {
        uint64_t x2 = x * x;
        uint64_t y2 = m - x2;
        uint64_t y  = _floor_isqrt_u64(y2);
        if (y * y != y2) continue;                          /* not a square */

        seq_t xf = (seq_t)x;
        seq_t yf = (seq_t)y;
        seq_t g  = gcd(xf, yf);
        seq_t f  = xf + yf - g;
        if (!*found || f < best) best = f;
        *found = true;
    }
    return best;
}

/* ------------------------------------------------------------------ */
/* cache for sequence values */
static seq_t *cache      = NULL;  /* array of a(1), a(2), ... */
static size_t cache_len  = 0;     /* number of terms already stored */
static size_t cache_cap  = 0;     /* allocated capacity of cache */
static uint64_t next_m   = 0;     /* next integer to test for representation */

/* enlarge cache capacity as needed */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();           /* out of memory */
    cache     = tmp;
    cache_cap = new_cap;
}

/* compute sequence values up to index n (1‑based) */
static void compute_up_to(idx_t n)
{
    while (cache_len < n) {
        uint64_t m = next_m;
        while (true) {
            bool found;
            seq_t f = min_f_for_m(m, &found);
            if (found) {
                grow_cache(cache_len + 1);
                cache[cache_len++] = f;
                next_m = m + 1;        /* next integer to test */
                break;
            }
            ++m;                       /* try next integer */
        }
    }
}

/* ------------------------------------------------------------------ */
/* public function as required by the problem */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined but harmless */
    compute_up_to(n);
    return cache[n - 1];
}
