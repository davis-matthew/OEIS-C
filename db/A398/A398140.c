/*
 * A398140  Numbers m such that the distinct prime factors of the product
 *          m*(m+1)*(m+2)*(m+3) are all consecutive primes beginning with 2
 *          (that is, the product is a term of A055932).
 * https://oeis.org/A398140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   factor_u64, is_prime_small, is_prime_small_acc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A398140
 *  Numbers m such that the distinct prime factors of m*(m+1)*(m+2)*(m+3)
 *  are all consecutive primes beginning with 2.
 *
 *  Implementation strategy:
 *      For each successive integer m (starting at 1) compute
 *      v = m*(m+3)/2.  m is a term iff v belongs to A141399 –
 *      i.e. v*(v+1) has as its distinct prime factors exactly the
 *      consecutive primes 2,3,5,…,p_k for some k.
 *
 *      To decide if v is in A141399 we factor v and v+1 using the
 *      provided helper factor_u64(), collect their distinct primes,
 *      find the maximum prime, and verify that every prime from 2 up to
 *      that maximum is present.  A simple trial‑division routine is
 *      used for primality testing of the small numbers involved.
 *
 *  The sequence is generated on demand and cached so that calls with
 *  arbitrary n values are answered efficiently.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"     /* factor_u64() */
#include "../utils/is_prime_small.h" /* is_prime_small() */
#include "../utils/is_prime_small_acc.h"

/* ---------- test if v belongs to A141399: v*(v+1) has consecutive
 *             primes starting with 2 */
static bool is_A141399(uint64_t v)
{
    uint64_t primes_v[128];
    uint64_t exps[128];
    size_t cnt_v = 0, cnt_vp = 0;

    /* factor v and v+1, concatenating their prime lists */
    factor_u64(v,     primes_v, exps, &cnt_v);
    factor_u64(v + 1, primes_v + cnt_v, exps + cnt_v, &cnt_vp);
    cnt_v += cnt_vp;

    /* find maximum prime factor */
    uint64_t max_p = 0;
    for (size_t i = 0; i < cnt_v; ++i)
        if (primes_v[i] > max_p) max_p = primes_v[i];

    /* verify that all primes up to max_p appear in the list */
    for (uint64_t p = 2; p <= max_p; ++p)
    {
        if (!is_prime_small_acc(p)) continue;          /* p is not prime – skip */
        bool found = false;
        for (size_t i = 0; i < cnt_v; ++i)
            if (primes_v[i] == p) { found = true; break; }
        if (!found) return false;                      /* missing prime */
    }
    return true;
}

/* ---------- cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_len   = 0;   /* number of stored terms */
static size_t cache_cap   = 0;   /* current capacity */
static uint64_t last_checked_m = 0;  /* last m examined */

/* ---------- resize cache if needed */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------- main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n = 0 */

    /* enlarge cache to hold n terms */
    ensure_cache_capacity((size_t)n);

    /* generate terms until we have at least n values */
    while (cache_len < (size_t)n)
    {
        ++last_checked_m;                     /* candidate m */

        /* compute v = m*(m+3)/2, using 128‑bit to avoid overflow */
        unsigned __int128 prod = (unsigned __int128)last_checked_m *
                                 (last_checked_m + 3ULL);
        uint64_t v = (uint64_t)(prod / 2);

        if (is_A141399(v))
        {
            cache[cache_len++] = (seq_t)last_checked_m;
        }
    }

    return cache[(size_t)(n - 1)];
}
