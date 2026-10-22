/*
 * A375398  Numbers k such that the minima of maximal anti-runs in the weakly
 *          increasing sequence of prime factors of k (with multiplicity) are
 *          distinct.
 * https://oeis.org/A375398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer such that its smallest prime factor
 *            occurs * exactly once and every other prime factor occurs at
 *            most twice. This * condition is equivalent to the minima of
 *            maximal anti‑runs in the weakly * increasing sequence of prime
 *            factors being distinct. * * The function generates th...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static seq_t *cache = NULL;          /* cached a(1) … a(len-1)          */
static size_t cache_cap = 0;         /* allocated capacity of cache      */
static idx_t  cache_len = 0;         /* number of terms currently cached */
static uint64_t last_checked = 0;    /* last integer that was tested      */

/* test whether k satisfies the defining property */
static bool is_valid(uint64_t k)
{
    if (k == 1) return true;        /* 1 has no prime factors */

    static uint64_t pr[64], ex[64]; /* temporary storage */
    size_t cnt = 0;
    factor_u64(k, pr, ex, &cnt);

    /* smallest prime factor must appear exactly once */
    if (cnt == 0 || ex[0] != 1) return false;

    /* all other prime factors may appear at most twice */
    for (size_t i = 1; i < cnt; ++i)
        if (ex[i] > 2) return false;

    return true;
}

/* ensure that cache contains at least n terms */
static void extend_cache(idx_t n)
{
    while (cache_len < n) {
        ++last_checked;                     /* next integer to test */
        if (is_valid(last_checked)) {
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) abort();    /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len] = (seq_t)last_checked;
            ++cache_len;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1 */
    extend_cache(n);
    return cache[n - 1];
}
