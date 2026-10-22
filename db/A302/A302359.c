/*
 * A302359  Numbers that are the sum of 3 squares > 1.
 * https://oeis.org/A302359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 169, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A302359 – Numbers that can be written as a^2 + b^2 + c^2
   with a,b,c ≥ 2. The function returns the n‑th such number (n ≥ 1).
   It precomputes all representations up to a fixed limit and then
   scans for the requested term. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* floor(sqrt(x)) for idx_t */
static idx_t floor_sqrt(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) <= x / (r + 1))
        ++r;
    return r;
}

/* static storage for precomputation */
static unsigned char *rep = NULL;      /* bitmap: 1 if representable */
static size_t rep_cap = 0;             /* size of rep array */
static bool initialized = false;

/* Precompute all sums a^2 + b^2 + c^2 ≤ LIMIT with a,b,c ≥ 2. */
static void precompute(void)
{
    if (initialized)
        return;
    initialized = true;

    /* Choose a limit that comfortably contains many terms. */
    const idx_t LIMIT = 200000;            /* 2×10^5 fits memory and time */
    rep_cap = (size_t)LIMIT + 1;
    rep = (unsigned char *)malloc(rep_cap * sizeof(unsigned char));
    if (!rep)
        abort();
    memset(rep, 0, rep_cap * sizeof(unsigned char));

    /* Compute squares for numbers ≥ 2 up to sqrt(LIMIT). */
    const idx_t max_sq = floor_sqrt(LIMIT);
    const size_t sq_len = (size_t)(max_sq - 1);   /* numbers 2..max_sq inclusive */
    idx_t *sqs = (idx_t *)malloc(sq_len * sizeof(idx_t));
    if (!sqs)
        abort();
    for (size_t i = 0; i < sq_len; ++i) {
        idx_t val = (idx_t)(i + 2);
        sqs[i] = val * val;
    }

    /* Mark all representable numbers.  Use a ≤ b ≤ c to avoid duplicates.
       Only existence matters, so we stop as soon as the total exceeds LIMIT. */
    for (size_t ai = 0; ai < sq_len; ++ai) {
        idx_t a2 = sqs[ai];
        for (size_t bi = ai; bi < sq_len; ++bi) {
            idx_t ab = a2 + sqs[bi];
            if (ab > LIMIT - 4)   /* need at least 2^2 = 4 for the third square */
                break;
            for (size_t ci = bi; ci < sq_len; ++ci) {
                idx_t total = ab + sqs[ci];
                if (total > LIMIT)
                    break;
                rep[(size_t)total] = 1;
            }
        }
    }
    free(sqs);
}

/* Return the n-th term (n ≥ 1). Undefined behavior if n < 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    precompute();                 /* ensure preprocessing has been done */

    idx_t count = 0;
    for (size_t i = 1; i < rep_cap; ++i) {
        if (rep[i]) {
            ++count;
            if (count == n)
                return (seq_t)i;
        }
    }
    /* If n exceeds the precomputed range, fallback: return 0. */
    return (seq_t)0;
}
