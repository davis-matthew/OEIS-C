/*
 * A326586  Odd numbers which do not satisfy Korselt's criterion, complement
 *          of A324050.
 * https://oeis.org/A326586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth odd composite number that does not satisfy Korselt's
 *            criterion (i.e., not a Carmichael number)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   factor_u64, grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/grow_seq.h"

/* static state for caching */
static seq_t *cache = NULL;
static size_t cache_size = 0;    /* number of elements cached */
static size_t cache_cap  = 0;    /* capacity of the cache */
static uint64_t cur = 1;         /* last odd number examined */

/* test whether a 64‑bit integer n is a Carmichael number */
static bool is_carmichael_u64(uint64_t n)
{
    if (n < 2)
        return false;
    if (is_prime(n))          /* primes are not Carmichael numbers */
        return false;

    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] != 1)        /* not square‑free */
            return false;
        uint64_t p = pr[i];
        if ((n - 1) % (p - 1) != 0)
            return false;     /* Korselt's criterion fails */
    }
    return true;                /* all conditions satisfied */
}

/* generate cached values up to index target (1‑based) */
static void ensure_up_to(idx_t target)
{
    if (cache_size >= (size_t)target)
        return;

    while (cache_size < (size_t)target) {
        uint64_t candidate;
        /* find next odd composite that is not Carmichael */
        do {
            cur += 2;                /* odd numbers only */
            candidate = cur;
            if (candidate <= 1)                /* skip 1, but not needed */
                continue;
            if (is_prime(candidate))            /* skip odd primes */
                continue;
            if (is_carmichael_u64(candidate))   /* skip Carmichael numbers */
                continue;
            break;                              /* found a member of the sequence */
        } while (1);

        /* enlarge cache if necessary */
        grow_seq(&cache, &cache_cap, cache_size + 1);
        cache[cache_size++] = (seq_t)candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for 0; return 0 as safe value */
        return 0;
    ensure_up_to(n);
    return cache[n - 1];
}
