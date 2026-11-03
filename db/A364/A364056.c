/*
 * A364056  Numbers whose prime factors have high median 2. Numbers whose
 *          prime factors (with multiplicity) are mostly 2's.
 * https://oeis.org/A364056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   build_spf, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) = nth integer whose multiset of prime factors has high median 2 */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/build_spf.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Static state for memoization */
    static seq_t *cache   = NULL;   /* array of found terms */
    static size_t cache_cap = 0;    /* current capacity of cache */
    static size_t cache_len = 0;    /* number of terms stored */

    static idx_t current = 2;        /* next integer to test */
    static idx_t *spf      = NULL;   /* smallest prime factor array */
    static idx_t spf_limit = 0;      /* size of the spf array */

    if (n == 0) return 0; /* offset starts at 1 */
    /* Generate terms until we have at least n terms cached */
    while (cache_len < n)
    {
        /* Ensure spf array covers the current number */
        if (current > spf_limit)
        {
            idx_t new_limit = spf_limit > 0 ? spf_limit * 2 : 1024;
            while (new_limit < current) new_limit *= 2;
            idx_t *new_spf = NULL;
            build_spf(new_limit, &new_spf);
            if (spf) free(spf);
            spf = new_spf;
            spf_limit = new_limit;
        }

        /* Factor current and count total factors (k) and factors of 2 (e2) */
        idx_t x = current;
        idx_t k  = 0;
        idx_t e2 = 0;
        while (x > 1)
        {
            idx_t p = spf[x];
            while (x % p == 0)
            {
                x /= p;
                k++;
                if (p == 2) e2++;
            }
        }
        /* high median of multiset is the (floor(k/2)+1)-th factor
           It equals 2 iff e2 ≥ floor(k/2)+1 */
        if (e2 >= (k / 2 + 1))
        {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)current;
        }
        current++; /* move to next integer for next iteration */
    }
    return cache[n-1];
}
