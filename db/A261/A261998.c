/*
 * A261998  Expansion of Product_{k>=1} (1-x^k)*(1+x^k)^4.
 * https://oeis.org/A261998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of ∏_{k≥1} (1−x^k)(1+x^k)^4, a(0)=1
 * Range:     u8 n <= 10, u16 n <= 32, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* 1‑D array storing all values up to the current maximum n requested. */
static seq_t *full = NULL;
static idx_t max_n = (idx_t)(-1);   /* no data yet */

/* Grow the array up to `limit` (inclusive) and recompute all values. */
static void recompute(idx_t limit)
{
    /* free old memory if any */
    free(full);

    /* allocate new array */
    full = (seq_t *)malloc((size_t)(limit + 1) * sizeof(seq_t));
    if (!full)
        return;  /* allocation failed; caller will handle zero */

    /* initialise */
    for (idx_t i = 0; i <= limit; ++i)
        full[i] = 0;

    full[0] = 1;                      /* product of zero factors = 1 */

    /* Main DP: multiply successively by (1−x^k)(1+x^k)^4 for k=1..limit */
    for (idx_t k = 1; k <= limit; ++k)
    {
        /* use a descending loop to keep the source values untouched
           while writing the destination values in place. */
        for (idx_t s = limit; ; --s)
        {
            seq_t val = full[s];                     /* a(s) */
            if (s >= k)
                val += (seq_t)3 * full[s - k];        /* +3*a(s‑k) */
            if (s >= 2 * k)
                val += (seq_t)2 * full[s - 2 * k];    /* +2*a(s‑2k) */
            if (s >= 3 * k)
                val -= (seq_t)2 * full[s - 3 * k];    /* −2*a(s‑3k) */
            if (s >= 4 * k)
                val -= (seq_t)3 * full[s - 4 * k];    /* −3*a(s‑4k) */
            if (s >= 5 * k)
                val -= full[s - 5 * k];               /* −a(s‑5k) */

            full[s] = val;                            /* store new value */
            if (s == 0)
                break;                                /* prevent underflow */
        }
    }

    max_n = limit;
}

/* External interface: returns a(n) for any n ≥ 0. */
seq_t a(idx_t n)
{
    if (full == NULL || n > max_n)
        recompute(n);

    /* If allocation failed, return zero as sentinel. */
    return (full && n <= max_n) ? full[n] : (seq_t)0;
}
