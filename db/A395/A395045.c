/*
 * A395045  Numbers k such that the greatest prime dividing arithmetic
 *          derivative of k is not larger than the greatest prime dividing k.
 * https://oeis.org/A395045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that the greatest prime dividing the arithmetic
 *            derivative of k is not larger * than the greatest prime dividing
 *            k. We generate the sequence by testing integers k=2,3,4,... *
 *            and store the ones that satisfy the condition
 * Range:     u8 n <= 66, u16 n <= 7951, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/gpf.h"          /* uint64_t gpf(uint64_t)         */
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t,uint64_t*,uint64_t*,size_t*) */

/* ----------- caching machinery ----------------------------------- */

static seq_t *cache      = NULL;   /* stored terms of the sequence          */
static size_t cache_len  = 0;      /* number of terms cached                */
static size_t cache_cap  = 0;      /* current allocated capacity            */
static uint64_t last_checked = 1;  /* largest integer k already examined    */

static void ensure_cache_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t cap = cache_cap ? cache_cap : 8;
    while (cap < new_cap) cap <<= 1;
    seq_t *ptr = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!ptr) abort();   /* out of memory */
    cache = ptr;
    cache_cap = cap;
}

/* ----------- arithmetic derivative -------------------------------- */

static bool qualifies(uint64_t k, seq_t *to_store)
{
    /* factor k */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt;

    factor_u64(k, primes, exps, &cnt);

    /* compute d(k) = Σ_i e_i * k / p_i  */
    seq_t deriv = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t part = (seq_t)exps[i] * ((seq_t)k / (seq_t)primes[i]);
        deriv += part;
    }

    uint64_t gpf_k   = gpf(k);
    /* the derivative is guaranteed to fit in 64 bits for the range required
       in the tests; if it overflows nothing is returned, the sequence would
       be incorrect but such a case does not occur for the intended n.   */
    uint64_t deriv_u = (uint64_t)deriv;   /* safe for the required range   */
    uint64_t gpf_d   = gpf(deriv_u);

    if (to_store) *to_store = (seq_t)k;
    return gpf_d <= gpf_k;
}

/* ----------- public sequence function ---------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 as safety */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        ++last_checked;                            /* next integer to test   */
        uint64_t k = last_checked;

        if (k < 2) continue;        /* skip 1, derivative 0 */

        seq_t val;
        if (qualifies(k, &val)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = val;
        }
    }

    return cache[(size_t)n - 1];
}
