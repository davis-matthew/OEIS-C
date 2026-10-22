/*
 * A362059  Total number of even divisors of all positive integers <= n.
 * https://oeis.org/A362059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = \sum_{k=1}^{\lfloor n/2 \rfloor} d(k)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where d(k) is the number of divisors of k.  This is equivalent to
   counting the total number of even divisors of all integers ≤ n. */

#include <stddef.h>
#include <stdlib.h>

/* static storage ------------------------------------------------------- */

static seq_t   *cum      = NULL; /* cumulative sums of d(k), cum[0] = 0   */
static size_t  cum_cap  = 0;     /* current capacity of cum array     */
static uint32_t *tau     = NULL; /* temporary array holding d(k)      */
static size_t  tau_cap   = 0;     /* current capacity of tau array    */

/* Forward declaration -------------------------------------------------- */

static void ensure_cumulative(idx_t m);

/* --------------------------------------------------------------------- */
/* Main sequence function ------------------------------------------------ */

seq_t a(idx_t n)
{
    /* floor(n/2) gives the upper limit of the sum   */
    idx_t m = n / 2;

    /* create or enlarge the cumulative array if needed */
    if (!cum || m >= (idx_t)cum_cap)
        ensure_cumulative(m);

    return cum[(size_t)m];
}

/* --------------------------------------------------------------------- */
/* Helper: build or extend the cumulative array ------------------------ */

static void ensure_cumulative(idx_t m)
{
    size_t new_cap = (size_t)m + 1;          /* indices 0 … m           */
    if (new_cap <= cum_cap)                  /* no change needed         */
        return;

    /* allocate temporary arrays */
    uint32_t *new_tau = (uint32_t *)calloc(new_cap, sizeof(uint32_t));
    seq_t    *new_cum = (seq_t    *)calloc(new_cap, sizeof(seq_t));

    if (!new_tau || !new_cum) {             /* allocation failed       */
        free(new_tau); free(new_cum);
        return; /* fall back: leave the old arrays untouched */
    }

    /* compute divisor counts d(k) for k = 1 .. m */
    for (size_t i = 1; i < new_cap; ++i)
    {
        for (size_t j = i; j < new_cap; j += i)
            new_tau[j]++;                    /* each divisor contributes 1   */
    }

    /* build cumulative sums cum[i] = \sum_{k=1}^{i} d(k) */
    new_cum[0] = 0;
    for (size_t i = 1; i < new_cap; ++i)
        new_cum[i] = new_cum[i - 1] + (seq_t)new_tau[i];

    /* replace old arrays (memory is freed; acceptable for short‑lived run) */
    free(tau);
    free(cum);
    tau     = new_tau;
    cum     = new_cum;
    tau_cap = new_cap;
    cum_cap = new_cap;
}
