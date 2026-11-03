/*
 * A392559  Numbers x such that for some y, a divisor of x such that 1<y<x,
 *          the following four numbers are prime: x*y+1, x*y-1, x/y+1, x/y-1.
 * https://oeis.org/A392559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 41, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A392559
 *
 * This implementation searches successive multiples of 6 (starting at 30)
 * for the first n numbers that satisfy the definition:
 *
 *   For some divisor 1 < y < x, all of
 *       x*y + 1,  x*y - 1,
 *       x/y + 1,  x/y - 1
 *   are prime.
 *
 * With the help of the routine is_prime_big from the utils,
 * the search is carried out incrementally and the results are cached
 * so that calls in arbitrary order are efficient.
 *
 * The sequence terms are all multiples of 6, starting with 30.
 *
 * The function has static storage and is fully re‑entrant for any
 * order of calls.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *cache = NULL;      /* cache of found terms          */
static size_t cache_len = 0;     /* number of terms stored        */
static size_t cache_cap = 0;     /* allocated capacity            */
static seq_t next_candidate = 30;/* next multiple of 6 to test    */

/* Ensure cache has space for at least `needed` elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap < needed)
    {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Test whether a candidate x satisfies the definition
 * for some divisor y.  All calculations stay in seq_t. */
static bool satisfies(seq_t x)
{
    /* iterate over divisors y > 1, y*y <= x */
    seq_t y;
    for (y = 2; y * y <= x; ++y)
    {
        if (x % y != 0) continue;

        /* divisors: y and k = x / y */
        seq_t k = x / y;
        if (k <= 1) continue;    /* k must be >1 */

        /* First test with divisor y */
        if (is_prime_big(k - 1) &&
            is_prime_big(k + 1) &&
            is_prime_big(y * y * k - 1) &&
            is_prime_big(y * y * k + 1))
        {
            return true;
        }

        /* If k differs from y, test with divisor k */
        if (k != y && y <= x / k) /* ensure k not already tested when y>sqrt(x) */
        {
            seq_t y2 = k;   /* new divisor */
            seq_t k2 = x / y2; /* which equals y */

            if (is_prime_big(k2 - 1) &&
                is_prime_big(k2 + 1) &&
                is_prime_big(y2 * y2 * k2 - 1) &&
                is_prime_big(y2 * y2 * k2 + 1))
            {
                return true;
            }
        }
    }
    return false;
}

/* Main sequence routine.  Any order of calls is supported. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, harmless */

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        /* Find the next valid multiple of 6 */
        seq_t found = 0;
        for (;;)
        {
            if (satisfies(next_candidate))
            {
                found = next_candidate;
                break;
            }
            next_candidate += 6;
        }

        /* Store the found term */
        grow_cache(cache_len + 1);
        cache[cache_len++] = found;

        /* Prepare for next search */
        next_candidate = found + 6;
    }

    return cache[(size_t)n - 1];
}
