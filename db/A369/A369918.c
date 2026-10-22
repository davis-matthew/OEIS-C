/*
 * A369918  Total area of all distinct rectangles with semiperimeter n whose
 *          side lengths are arithmetic numbers.
 * https://oeis.org/A369918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..⌊n/2⌋} k*(n-k)*c(k)*c(n-k) , where c(m)=1 if m
 *            is an arithmetic number (sigma(m) divisible by tau(m)), else 0
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static storage */
static bool *arith     = NULL;   /* arithmetic indicator, index 1..N */
static size_t arith_cap = 0;     /* size of arithmetic array (N+1) */
static seq_t *area     = NULL;   /* precomputed a(n) values, index 0..N */
static idx_t  area_cap  = 0;     /* number of computed a(n), i.e. last index */

/* ------------------------------------------------------------------ */
/*  Compute the arithmetic indicator array up to 'limit' (inclusive). */
static void recompute_arith(idx_t limit)
{
    /* allocate temporary arrays for tau and sigma */
    uint32_t *tau   = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    uint64_t *sigma = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    if (!tau || !sigma) {
        free(tau);
        free(sigma);
        return;
    }

    /* initialise to zero */
    for (idx_t i = 0; i <= limit; ++i) {
        tau[i]   = 0;
        sigma[i] = 0;
    }

    /* classic divisor-sum sieve */
    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; j += i) {
            ++tau[j];
            sigma[j] += i;
        }
    }

    /* allocate new arithmetic array */
    bool *new_arith = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!new_arith) {
        free(tau);
        free(sigma);
        return;
    }

    for (idx_t i = 1; i <= limit; ++i) {
        new_arith[i] = (sigma[i] % tau[i] == 0);
    }

    /* replace old array */
    free(arith);
    arith     = new_arith;
    arith_cap = limit + 1;

    /* clean up temporaries */
    free(tau);
    free(sigma);
}

/* ------------------------------------------------------------------ */
/*  Compute the sequence values up to 'limit' (inclusive).           */
static void recompute_area(idx_t limit)
{
    /* allocate new area array */
    seq_t *new_area = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!new_area) return;

    /* free old array if any */
    free(area);
    area     = new_area;
    area_cap = limit + 1;

    /* compute a(0)=0 explicitly */
    if (limit >= 0)
        area[0] = (seq_t)0;

    /* compute for each n from 1 to limit */
    for (idx_t n = 1; n <= limit; ++n) {
        seq_t sum = 0;
        idx_t half = n >> 1;          /* floor(n/2) */
        for (idx_t k = 1; k <= half; ++k) {
            if (arith[k] && arith[n - k]) {
                sum += (seq_t)k * (seq_t)(n - k);
            }
        }
        area[n] = sum;
    }
}

/* ------------------------------------------------------------------ */
/*  Public sequence function.                                         */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* If we have not computed up to n, extend the tables. */
    if (n >= area_cap || n >= arith_cap) {
        idx_t new_max = n;
        recompute_arith(new_max);
        recompute_area(new_max);
    }

    return area[n];
}
