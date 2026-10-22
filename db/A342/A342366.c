/*
 * A342366  a(1) = 1, a(2) = 2; for n > 2, a(n) is the least positive integer
 *          not occurring earlier that shares a factor but not a digit with
 *          a(n-1).
 * https://oeis.org/A342366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A342366
   a(1) = 1, a(2) = 2; for n > 2, a(n) is the least positive integer
   not appearing earlier that has a common factor with a(n‑1) but
   shares no digit with a(n‑1). */

/* This implementation builds the sequence on demand, memoising all
   previously computed terms in a static cache. It uses the generic
   gcd routine supplied by the oeis-build utilities. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/gcd.h"   /* provides seq_gcd(x, y) and/or gcd(x, y) */

/* Compute the mask of decimal digits present in x.  Bits 0–9 correspond
   to digits 0–9.  The function works for any seq_t value. */
static inline uint16_t digit_mask(seq_t x)
{
    uint16_t mask = 0;
    if (x == 0)
        return 1u << 0;
    while (x > 0)
    {
        seq_t d = x % (seq_t)10;
        mask |= 1u << (int)d;
        x /= (seq_t)10;
    }
    return mask;
}

/* Sequence producing function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of a(1)…a(k) */
    static size_t cache_size = 0;    /* number of cached terms */
    static size_t cache_cap = 0;     /* allocated capacity of cache */
    static uint8_t *used = NULL;     /* flag array marking numbers used */
    static size_t used_cap = 0;      /* capacity of used array */

    if (n == 0)                     /* undefined, but protect */
        return (seq_t)0;

    /* First initialisation on first call */
    if (cache == NULL)
    {
        /* ready for two terms (a1=1, a2=2) */
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_size = 2;

        /* used array needs indices up to 2 */
        used_cap = 3;
        used = (uint8_t *)calloc(used_cap, sizeof(uint8_t));
        used[1] = 1;   /* 1 is used */
        used[2] = 1;   /* 2 is used */
    }

    /* Grow cache to hold n terms if necessary */
    if ((size_t)n > cache_size)
    {
        /* Ensure cache has enough capacity */
        if ((size_t)n > cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : (size_t)n;
            while (new_cap < (size_t)n) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0; /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute terms from cache_size+1 up to n */
        for (idx_t k = cache_size + 1; k <= n; ++k)
        {
            seq_t prev = cache[k - 2];           /* a(k-1) */
            uint16_t prev_digits = digit_mask(prev);
            seq_t cand = 1;

            for (;; ++cand)
            {
                /* Ensure the used array covers the candidate's index */
                size_t cand_idx = (size_t)cand;
                if (cand_idx >= used_cap)
                {
                    size_t new_used_cap = used_cap ? used_cap * 2 : cand_idx + 1;
                    while (new_used_cap <= cand_idx) new_used_cap *= 2;
                    uint8_t *tmp = (uint8_t *)realloc(used, new_used_cap);
                    if (!tmp) return (seq_t)0; /* allocation failure */
                    /* initialise the added part to 0 */
                    memset(tmp + used_cap, 0, (new_used_cap - used_cap) * sizeof(uint8_t));
                    used = tmp;
                    used_cap = new_used_cap;
                }

                if (used[cand_idx])               /* already used earlier */
                    continue;

                /* Check for a shared factor */
                seq_t g = gcd(prev, cand);        /* namespace collision avoided by included header */
                if (g <= (seq_t)1)
                    continue;

                /* Check for digit overlap */
                uint16_t cand_digits = digit_mask(cand);
                if (prev_digits & cand_digits)
                    continue;

                /* We found the next term */
                cache[k - 1] = cand;
                used[cand_idx] = 1;
                break;
            }
        }
        cache_size = (size_t)n;
    }

    return cache[n - 1];
}
