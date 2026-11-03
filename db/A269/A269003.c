/*
 * A269003  Numbers having the same parity of numbers of 1's in bases 2 and
 *          -2.
 * https://oeis.org/A269003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth non‑negative integer such that the parity of the
 *            count of 1's in its binary (base 2) representation equals the
 *            parity of the count of 1's in its balanced‑negabinary (base −2)
 *            representation. The function uses a lazy extendable cache so
 *            calls can be in any order
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 60 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* parity of number of 1s in binary representation of x (true = odd, false = even) */
static bool parity_bin(seq_t x)
{
    bool p = false;
    while (x != 0) {
        x = x & (x - 1);   /* drop the lowest set bit */
        p = !p;
    }
    return p;
}

/* parity of number of 1s in base‑(-2) representation of x */
static bool parity_neg2(seq_t x)
{
    /* For the algorithm we need a signed integer that can hold intermediate
       negative values.  The values of a(n) for all requested n are far less
       than 2^63, so it is safe to cast to int64_t. */
    int64_t n = (int64_t)x;
    bool p = false;
    while (n != 0) {
        int r = n & 1;           /* remainder 0 or 1 */
        if (r) p = !p;
        n = (n - r) / -2;        /* next co‑efficient in base -2 */
    }
    return p;
}

/* static cache for previously computed terms */
static seq_t *cache = NULL;      /* array of already found terms */
static size_t cache_size = 0;    /* number of cached terms */
static size_t cache_cap   = 0;    /* allocated size of cache */

/* enlarge cache if necessary */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= need) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) exit(1);          /* abort on allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Grow cache until it contains a(n) */
    while (n >= (idx_t)cache_size) {
        ensure_cache(cache_size + 1);

        /* start searching from the next integer after the last cached term */
        seq_t cand = cache_size ? cache[cache_size - 1] + 1 : 0;
        bool found = false;
        while (!found) {
            /* compute parity of binary and negabinary representations */
            if (parity_bin(cand) == parity_neg2(cand)) {
                cache[cache_size++] = cand;
                found = true;
            }
            cand = cand + 1;
        }
    }
    return cache[n];
}
