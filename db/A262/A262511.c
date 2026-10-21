/*
 * A262511  Numbers k for which there is exactly one solution to x - d(x) = k,
 *          where d(k) is the number of divisors of k (A000005). Positions of
 *          ones in A060990.
 * https://oeis.org/A262511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) are the numbers k for which there is exactly one solution *
 *            to x - d(x) = k. The function precomputes all such k up to a *
 *            fixed bound using a divisor‑sieve, records the values that *
 *            occur exactly once, and returns the n‑th smallest
 * Range:     u8 n <= 104, u16 n <= 9562, u32 n <= 9562, u64 n <= 9562,
 *            u128 n <= 9562, big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/append_candidate.h"

/* --------------------------------------------------------------------- */
/*  Precomputation parameters                                           */
/* --------------------------------------------------------------------- */
static const idx_t MAX_X = 5000000;          /* upper bound for x   */

static bool init_done   = false;
static uint32_t *div_cnt = NULL;            /* divisor counts for 1..MAX_X           */
static uint8_t *k_cnt   = NULL;             /* occurrence count of k values         */
static seq_t *unique_k  = NULL;             /* list of k that occur exactly once    */
static size_t unique_len = 0;               /* number of such k values              */

/* --------------------------------------------------------------------- */
/*  Initialize data structures                                            */
/* --------------------------------------------------------------------- */
static void init(void)
{
    if (init_done) return;

    /* Allocate and compute divisor counts */
    div_cnt = (uint32_t*)calloc((size_t)MAX_X + 1, sizeof(uint32_t));
    if (!div_cnt) return;

    for (idx_t i = 1; i <= MAX_X; ++i)
        for (idx_t j = i; j <= MAX_X; j += i)
            ++div_cnt[j];

    /* Allocate k counter array */
    k_cnt = (uint8_t*)calloc((size_t)MAX_X + 1, sizeof(uint8_t));
    if (!k_cnt) { free(div_cnt); div_cnt = NULL; return; }

    /* Count occurrences of each k = x - d(x) */
    for (idx_t x = 1; x <= MAX_X; ++x) {
        idx_t k = x - (idx_t)div_cnt[x];
        if (k <= MAX_X) {
            if (k_cnt[k] < 255)  /* avoid overflow of uint8_t */
                ++k_cnt[k];
        }
    }

    /* Build the list of k that occur exactly once */
    size_t cap = 0;
    for (idx_t k = 2; k <= MAX_X; ++k)
        if (k_cnt[k] == 1)
            append_candidate(&unique_k, &unique_len, &cap, (seq_t)k);

    init_done = true;
}

/* --------------------------------------------------------------------- */
/*  Public sequence function                                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined for n < 1 */
    init();
    if ((idx_t)(n - 1) < unique_len)
        return unique_k[n - 1];
    return 0;                 /* not reached for supported range */
}
