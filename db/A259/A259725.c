/*
 * A259725  Numbers k such that [r[s*k]] = [s[r*k]], where r = sqrt(2),
 *          s=sqrt(3), and [ ] = floor.
 * https://oeis.org/A259725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 51, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A259725 - numbers k such that ⎣√2·⎣√3·k⎦⎦ = ⎣√3·⎣√2·k⎦⎦
   The function returns the n‑th such number (1‑based).  For efficiency
   it keeps a static cache of already discovered terms and extends it
   incrementally.  sqrt is computed by an integer binary search to avoid
   floating‑point inaccuracies. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Integer square root: floor(sqrt(a)) for any unsigned integral type. */
static seq_t int_sqrt(seq_t a)
{
    seq_t low = 0, high = a;
    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;
        if (mid <= a / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* cache of the sequence terms (a(1) stored at cache[0]) */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;

/* ensure the cache has space for at least newcap elements */
static void ensure_cache(size_t newcap)
{
    if (cache_cap >= newcap)
        return;
    size_t newsize = cache_cap ? cache_cap * 2 : 16;
    while (newsize < newcap)
        newsize <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
    if (!tmp)
        abort();                  /* out of memory */
    cache = tmp;
    cache_cap = newsize;
}

/* test whether the candidate k satisfies the defining equality */
static bool good_k(idx_t k)
{
    seq_t kk = (seq_t)k;
    seq_t k2 = kk * kk;               /* k² */

    seq_t X = int_sqrt((seq_t)3 * k2); /* ⎣√3·k⎦ = ⎣√(3k²)⎦ */
    seq_t Y = int_sqrt((seq_t)2 * k2); /* ⎣√2·k⎦ = ⎣√(2k²)⎦ */

    seq_t a1 = int_sqrt((seq_t)2 * X * X); /* ⎣√2·X²⎦ */
    seq_t a2 = int_sqrt((seq_t)3 * Y * Y); /* ⎣√3·Y²⎦ */

    return a1 == a2;
}

/* external linkage as requested */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset 1 – return 0 for safety */
        return (seq_t)0;

    /* find the next good candidate(s) until we have n terms */
    while (cache_len < (size_t)n) {
        while (!good_k(next_candidate))
            ++next_candidate;                /* skip non‑good values */

        ensure_cache(cache_len + 1);
        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;                    /* prepare next search */
    }

    return cache[n - 1];
}
