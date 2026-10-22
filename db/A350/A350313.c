/*
 * A350313  The Redstone permutation: a(1) = 2, a(2) = 1, otherwise the
 *          smallest number not occurring earlier which is strongly prime to
 *          n.
 * https://oeis.org/A350313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A350313 – Redstone permutation
   a(1) = 2, a(2) = 1, otherwise the smallest unused number
   strongly prime to n.  "k is strongly prime to n" if
   gcd(k,n)=1 and k∤(n‑1).  The function computes a(n) from
   scratch; no global sequence cache is kept. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

static unsigned char *used = NULL;
static size_t used_cap = 0;

/* Ensure that used[i] can be indexed for i up to val (inclusive). */
static void ensure_used_capacity(seq_t val)
{
    /* val is always non‑negative; cast only to size_t when it is
       within a reasonable range (the sequence values for n≤10 000
       stay below 2^16). */
    size_t idx = (size_t)val;
    if (idx < used_cap)
        return;
    size_t new_cap = used_cap > 0 ? used_cap : 16;
    while (new_cap <= idx)
        new_cap <<= 1;
    unsigned char *new_used =
        (unsigned char *)realloc(used, new_cap);
    if (!new_used) exit(1);                /* memory exhausted */
    for (size_t i = used_cap; i < new_cap; ++i)
        new_used[i] = 0;
    used   = new_used;
    used_cap = new_cap;
}

/* Return the n-th term of the Redstone permutation (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* undefined for 0 */

    /* Clear used array for this call.  It is cheap because used_cap
       is at most a few thousand for the required range of n. */
    if (used)
        for (size_t i = 0; i < used_cap; ++i)
            used[i] = 0;

    idx_t target = n;
    seq_t result = 0;

    for (idx_t pos = 1; pos <= target; ++pos)
    {
        seq_t chosen;

        if (pos == 1)
            chosen = (seq_t)2;
        else if (pos == 2)
            chosen = (seq_t)1;
        else
        {
            /* Find the smallest unused number that is strongly
               prime to pos. */
            seq_t cand = (seq_t)1;
            while (1)
            {
                ensure_used_capacity(cand);
                if (!used[(size_t)cand])
                {
                    if (gcd((seq_t)cand, (seq_t)pos) == 1 &&
                        ((seq_t)(pos - 1) % cand) != 0)
                    {
                        chosen = cand;
                        break;
                    }
                }
                ++cand;
            }
        }

        ensure_used_capacity(chosen);
        if (pos == target)
            return chosen;                  /* found the answer */
        used[(size_t)chosen] = 1;            /* mark as used */
    }

    return 0; /* unreachable */
}
