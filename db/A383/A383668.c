/*
 * A383668  Numbers whose binary representation has a positive number of 0's,
 *          all with even runlength.
 * https://oeis.org/A383668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 2567, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9362 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A383668 - numbers whose binary representation has a
   positive number of 0's, each zero run having even length.
   The function generates terms on demand and memoizes them. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether a given integer x meets the even‑zero‑run condition. */
static bool good_zero_runs(seq_t x)
{
    bool hasZero = false;
    size_t run = 0;

    while (x)
    {
        if (x & 1)
        {
            if (run)
            {
                if (run & 1)        /* odd length -> reject */
                    return false;
                run = 0;
            }
        }
        else
        {
            hasZero = true;
            run++;
        }
        x >>= 1;
    }

    if (run & 1)                /* trailing zero run odd -> reject */
        return false;

    return hasZero;             /* must contain at least one zero */
}

/* Return the n-th term of A383668 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined, but avoid UB */

    /* static state to support arbitrary call order */
    static seq_t *cache = NULL;
    static size_t cap  = 0;
    static size_t size = 0;     /* number of terms stored in cache */
    static idx_t cand  = 4;     /* next candidate integer to test */

    /* Grow cache if needed and generate missing terms */
    if (size < n)
    {
        if (!cache)
        {
            cap = 16;
            cache = mem_new(seq_t, cap);
        }

        while (size < n)
        {
            /* Find next qualifying integer */
            while (!good_zero_runs((seq_t)cand))
                cand++;

            /* Expand cache when full */
            if (size == cap)
            {
                cap <<= 1;   /* double capacity */
                cache = mem_resize(seq_t, cache, cap);
            }

            cache[size++] = (seq_t)cand; /* store term */
            cand++;                       /* advance for next search */
        }
    }

    return cache[n - 1];
}
