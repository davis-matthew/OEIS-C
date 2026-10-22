/*
 * A325776  a(n) is the least nonnegative integer such that n concatenated
 *          with a(n) is a boomerang number (A308306), or -1 if n is a
 *          boomerang number.
 * https://oeis.org/A325776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 98, i16 n <= 7998, i32 n <= 9563, i64 n <= 9563,
 *            i128 n <= 9563, big any n
 * Helpers:   displacement
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 
 *   -1, if disp(n) = 0          (n itself is a boomerang number)
 *   min{m >= 0 | disp(m) = -disp(n)}, otherwise
 * where disp(x) = sum over decimal digits d of (d even? +d+1 : -(d+1)).
 * This function computes disp(n) and lazily caches the minimal m for each
 * possible target displacement in the [%−OFFSET, +OFFSET] range.
 * Since for n ≤ 10 000 the magnitude of disp(n) never exceeds 40, the
 * OFFSET of 2000 is more than sufficient for all practical indices.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/displacement.h"

#define OFFSET 2000  /* ensures all needed displacements are covered */

static seq_t dp_cache[2 * OFFSET + 1];  /* minimal m for each displacement */
static bool dp_initialized = false;

/* Ensure the cache is initialized to the sentinel value (seq_t)-1. */
static void init_cache(void)
{
    for (int i = 0; i < 2 * OFFSET + 1; ++i)
        dp_cache[i] = (seq_t)-1;
    dp_initialized = true;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    if (!dp_initialized)
        init_cache();

    seq_t d = displacement((seq_t)n);
    if (d == 0)         /* n itself is a boomerang number   */
        return (seq_t)-1;

    seq_t target = -d;           /* we need a number with disp = -d   */
    int idx = (int)(target + OFFSET);
    if (idx < 0 || idx > 2 * OFFSET)  /* out-of-range: compute directly   */
    {
        seq_t m = 0;
        while (true)
        {
            if (displacement(m) == target)
                return m;
            ++m;
        }
    }

    if (dp_cache[idx] != (seq_t)-1)   /* cached minimal m already known   */
        return dp_cache[idx];

    /* Compute minimal m for this target (cache the result). */
    seq_t m = 0;
    while (true)
    {
        if (displacement(m) == target)
        {
            dp_cache[idx] = m;     /* store for future queries   */
            return m;
        }
        ++m;
    }
}
