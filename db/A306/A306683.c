/*
 * A306683  Integers k for which the base-phi representation of k does not
 *          include 1 or phi.
 * https://oeis.org/A306683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  Enumerate natural numbers, skipping those whose base‑phi
 * representation contains digit 1 or φ.  The numbers with digit 1 form the
 * Beatty sequence  b(n)=⌊n·φ⌋+2n+1  (n≥0).  The numbers with digit φ form
 * the Beatty sequence  c(n)=⌊n·φ⌋+2n-1  (n≥1).  The remaining integers
 * comprise a(n).  We generate the sequence on demand and keep a static
 * cache so repeated calls are O(1) after the first one.  The product
 * ⌊n·φ⌋ is computed without floating point by the identity
 *   ⌊n·φ⌋ = ⌊(n + ⌊n√5⌋)/2⌋ ,
 * where ⌊n√5⌋ = ⌊√(5·n²)⌋ is obtained by an integer square‑root
 * routine from the utility library.  Only integer arithmetic is used.   */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Compute ⌊n·φ⌋ using an integer square‑root. */
static inline seq_t floor_phi(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sq     = nn * nn;           /* n² */
    seq_t sq5    = sq * 5;            /* 5·n²  */
    seq_t sroot  = floor_sqrt(sq5);   /* ⌊n·√5⌋  */
    return (nn + sroot) >> 1;         /* (nn + sroot)/2  */
}

/* Beatty sequence b(n) = ⌊n·φ⌋ + 2n + 1  (n≥0). */
static inline seq_t compute_b(idx_t n)
{
    return floor_phi(n) + ((seq_t)n << 1) + 1;
}

/* Beatty sequence c(n) = ⌊n·φ⌋ + 2n - 1  (n≥1). */
static inline seq_t compute_c(idx_t n)
{
    return floor_phi(n) + ((seq_t)n << 1) - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined below offset 1 */

    /* Static cache for computed a‑values. */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* Current state of the exclusion sequences. */
    static idx_t  idx_b      = 0;     /* B starts at n=0 → 1  */
    static idx_t  idx_c      = 1;     /* C starts at n=1 → 2  */
    static seq_t b_val = 0, c_val = 0;
    static idx_t  current = 1;        /* integer being inspected */

    /* One‑time initialisation. */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        b_val = compute_b(idx_b);
        c_val = compute_c(idx_c);
    }

    /* Generate values until we have at least n entries. */
    while (cache_size < n) {
        if (current == b_val) {                 /* skip numbers from b */
            ++idx_b;
            b_val = compute_b(idx_b);
        } else if (current == c_val) {          /* skip numbers from c */
            ++idx_c;
            c_val = compute_c(idx_c);
        } else {                                /* a valid a‑value */
            if (cache_size == cache_cap) {      /* grow cache if needed */
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache,
                                          sizeof(seq_t) * cache_cap);
            }
            cache[cache_size++] = current;
        }
        ++current;
    }

    return cache[n - 1];
}
