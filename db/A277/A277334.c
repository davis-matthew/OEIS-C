/*
 * A277334  Numbers n, that apart from 2 are all odd and for which n/(largest
 *          prime dividing n) is squarefree.
 * https://oeis.org/A277334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, good_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n‑th integer that
 *   1) is 1 or 2, or otherwise an odd integer;
 *   2) in its prime factorisation all exponents are 1,
 *      except possibly the largest prime, which may have
 *      exponent 1 or 2.
 * The function builds a small SPF (smallest prime factor) sieve
 * to test numbers quickly and iterates until the desired
 * term is found.  It works for any order of calls.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_spf.h"
#include "../utils/good_odd.h"

static idx_t *g_spf = NULL;
static idx_t  g_spf_limit = 0;

/* Ensure that g_spf covers numbers up to `limit`. */
static void ensure_spf(idx_t limit)
{
    if (g_spf && g_spf_limit >= limit)
        return;
    if (g_spf)
        free(g_spf);
    build_spf(limit, &g_spf);
    g_spf_limit = limit;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)2;

    idx_t count     = 2;      /* 1 and 2 already counted */
    idx_t candidate = 3;
    idx_t limit     = 100000; /* initial sieve size */
    ensure_spf(limit);

    while (1)
    {
        if (candidate & 1ULL) /* odd candidates only */
        {
            if (good_odd(candidate, g_spf))
            {
                count++;
                if (count == n)
                    return (seq_t)candidate;
            }
        }
        candidate++;
        if (candidate > limit)
        {
            limit <<= 1;    /* double the sieve size */
            ensure_spf(limit);
        }
    }
}
