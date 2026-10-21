/*
 * A291892  Number of symmetrically unique Dyck paths of semilength n and
 *          height eight.
 * https://oeis.org/A291892
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 13, u32 n <= 21, u64 n <= 38, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A291892 : Number of symmetrically unique Dyck paths of semilength n and height eight
    Recurrence (valid for n ≥ 23):
        a(n) = 15*a(n-1) - 84*a(n-2) + 178*a(n-3) + 160*a(n-4)
             - 1436*a(n-5) + 1985*a(n-6) + 1447*a(n-7) - 6220*a(n-8)
             + 4096*a(n-9) + 3445*a(n-10) - 5665*a(n-11) + 1925*a(n-12)
             + 405*a(n-13) - 275*a(n-14) + 25*a(n-15)
    Initial values (offset 8):
        a(8) = 1,  a(9) = 8,  a(10) = 71,  a(11) = 461,  a(12) = 2706,
        a(13) = 14235,  a(14) = 70161,  a(15) = 327469,  a(16) = 1469596,
        a(17) = 6389144,  a(18) = 27097948,  a(19) = 112630404,
        a(20) = 460511702,  a(21) = 1857372265,  a(22) = 7406424903
    The function memoises computed values and works for calls in any order.
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static const int coeff[15] = {15, -84, 178, 160, -1436, 1985, 1447, -6220,
                               4096, 3445, -5665, 1925, 405, -275, 25};

static const seq_t base[15] = {1, 8, 71, 461, 2706, 14235, 70161, 327469,
                               1469596, 6389144, 27097948, 112630404,
                               460511702, 1857372265, 7406424903};

static seq_t *cache = NULL;      /* memoisation array */
static size_t cache_size = 0;    /* allocated capacity (elements) */
static idx_t max_computed = 0;   /* highest index already available */

static void
ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    size_t new_cap = (cache_size == 0) ? 16 : cache_size;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();

    /* initialise new entries to zero */
    for (size_t i = cache_size; i < new_cap; ++i)
        new_cache[i] = 0;

    /* fill in the known initial values (a(8) … a(22)) */
    for (size_t i = cache_size; i < new_cap && i <= 22; ++i)
    {
        if (i >= 8)
            new_cache[i] = base[i - 8];
    }

    cache = new_cache;
    cache_size = new_cap;

    /* ensure that we consider the first 15 terms already computed */
    if (max_computed < 22)
        max_computed = 22;
}

seq_t a(idx_t n)
{
    if (n < 8)
        return 0;

    if (n <= 22)
        return base[n - 8];

    ensure_cache(n);

    if (max_computed < n)
    {
        for (idx_t k = max_computed + 1; k <= n; ++k)
        {
            /* recurrence uses indices k-1 … k-15 */
            seq_t pos = 0;
            seq_t neg = 0;

            for (int i = 1; i <= 15; ++i)
            {
                int   c   = coeff[i - 1];
                idx_t idx = k - i;
                seq_t prod = cache[idx] *
                              (seq_t)(c >= 0 ? (unsigned int)c : (unsigned int)(-c));

                if (c >= 0)
                    pos += prod;
                else
                    neg += prod;
            }

            cache[k] = pos - neg;   /* assume pos ≥ neg; for the sequence this holds */
        }
        max_computed = n;
    }

    return cache[n];
}
