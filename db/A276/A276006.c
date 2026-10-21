/*
 * A276006  Numbers with at least one hit in their factorial base
 *          representations (see comments for the definition); positions of
 *          nonzeros in A276004 & A276007.
 * https://oeis.org/A276006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 184, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A276006 – numbers with at least one hit in their factorial base representation.
 *  The implementation treats the sequence as a monotone list and caches
 *  each term as it is discovered.  For each new integer the factorial‑base
 *  representation is examined directly; a hit is found when there exists a
 *  pair of non‑zero digits d_i (position i, 1‑based) and d_j (j < i) with
 *  (i – d_i) = j.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef __cplusplus
/*  For big‑integer builds the array must be allocated with the provided
 *  helper which performs construction of each element.                       */
#include "../utils/seq_alloc.h"
#endif

/* Maximum number of terms kept in the cache; sufficient for n up to about
 * 100 000 (more than enough for the required 10 000).  The cache size may
 * be increased without changing the algorithm.                           */
#define CACHE_CAP 200000

/*  Helper that tests whether a given number has a hit in its factorial
 *  base representation.                                                     */
static bool has_hit(seq_t x)
{
    /*  Extract factorial‑base digits (least significant digit at position 1).
     *  digit i (0‑based) = tmp % (i+1), then tmp /= (i+1).                    */
    uint8_t dig[256];
    size_t len = 0;
    seq_t tmp = x;

    for (size_t i = 0; tmp != 0; ++i)
    {
        seq_t denom = (idx_t)(i + 1);
        seq_t d = tmp % denom;
        tmp  /= denom;
        if (len < sizeof(dig))
            dig[len++] = (uint8_t)d;
        else
            break;   /*  safety – should never happen for numbers in the range */
    }

    /*  Search for a hit: find i>j such that i+1 (1‑based) – d_i = j+1 (1‑based)*/
    for (size_t i = 0; i < len; ++i)
    {
        if (dig[i] == 0)
            continue;
        for (size_t j = 0; j < i; ++j)
        {
            if (dig[j] == 0)
                continue;
            if (i + 1 - dig[i] == j + 1)
                return true;
        }
    }
    return false;
}

/*  Cached sequence data ----------------------------------------------------- */
static seq_t          *cache      = NULL;   /* cache array of terms           */
static size_t          cache_len  = 0;     /* number of terms stored         */
static seq_t           next_value = 3;     /* next integer to test           */

/*  Allocate the cache on first use.  For big builds seq_alloc constructs
 *  each element; for standard builds malloc is sufficient.                */
static void init_cache(void)
{
    if (cache != NULL)
        return;
#ifdef __cplusplus
    cache = seq_alloc(CACHE_CAP);
#else
    cache = (seq_t *)malloc(CACHE_CAP * sizeof(seq_t));
#endif
    /*  No error handling required – the build environment ensures success. */
}

/*  Public sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)           /* the sequence has offset 1; behaviour for n<1 is
                             undefined – we simply return 0.              */
        return 0;

    init_cache();

    /*  Build the cache up to index n (1‑based).                            */
    while (cache_len < (size_t)n)
    {
        if (has_hit(next_value))
        {
            cache[cache_len++] = next_value;
        }
        ++next_value;
    }

    return cache[(size_t)n - 1];
}
