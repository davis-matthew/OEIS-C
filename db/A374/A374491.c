/*
 * A374491  Decimal expansion of zeta(-11).
 * https://oeis.org/A374491
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = decimal digit at position n+2 of zeta(-11) = 691/32760
 * Range:     u8 n <= 0, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* numerator and denominator of the exact value of zeta(-11) */
static const seq_t Z_691 = 691;
static const seq_t Z_32760 = 32760;

/* cache for already computed decimal digits (0‑indexed after the point) */
static idx_t     cache_size = 0;   /* number of digits stored */
static idx_t     cache_cap  = 0;   /* current allocation capacity */
static seq_t *   cache      = NULL;

/* remainder for the next digit computation */
static uint64_t  rem_val    = 0;

/* Ensure that digits 0 … pos are computed and stored in cache */
static void
ensure_digits(idx_t pos)
{
    if (cache == NULL) {
        /* first time: allocate room for a few digits */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        rem_val = (uint64_t)Z_691; /* initial remainder = numerator */
    }

    /* compute digits up to and including position 'pos' */
    while (cache_size <= pos) {
        uint64_t prod  = rem_val * 10ULL;                       /* rem*10      */
        seq_t    digit = (seq_t)(prod / (uint64_t)Z_32760);      /* floor division */
        rem_val = prod % (uint64_t)Z_32760;                    /* new remainder */

        /* grow cache if necessary */
        if (cache_size >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_size++] = digit;
    }
}

seq_t a(idx_t n)
{
    /* n = -1 gives the first supplied term, n >= -1 is the valid range   */
    idx_t pos = n + 2;                         /* digit index after point */
    ensure_digits(pos);
    return cache[(size_t)pos];
}
