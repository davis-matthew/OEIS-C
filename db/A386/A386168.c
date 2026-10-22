/*
 * A386168  Primes having only {2, 7, 8, 9} as digits.
 * https://oeis.org/A386168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 79, u32 n <= 48, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the nth prime whose decimal digits are only 2, 7, 8 or 9. */
/*  The implementation generates the admissible numbers in increasing
 *  order and keeps a cache of the primes found.  When a(n) is called
 *  the cache is extended until it contains at least n terms.       */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/ensure_capacity.h"
#include "../utils/is_prime_big.h"

/* digits allowed in a term */
static const unsigned char allowed[4] = {2, 7, 8, 9};

/* state of the number generator – works like a counter in base 4 */
static size_t gen_idx[20] = {0};   /* indices into allowed digit array   */
static size_t gen_len = 1;         /* current number of digits           */

/* return the next admissible integer (with digits from allowed[])
 * and increment the internal counter. */
static seq_t next_candidate(void)
{
    seq_t val = 0;
    for (size_t i = 0; i < gen_len; ++i)
        val = val * 10 + allowed[gen_idx[i]];

    /* increment counter in base‑4 */
    size_t pos = gen_len - 1;
    while (1) {
        ++gen_idx[pos];
        if (gen_idx[pos] < 4)
            break;
        gen_idx[pos] = 0;
        if (pos == 0) {                /* overflow -> increase length   */
            ++gen_len;
            if (gen_len > 20)          /* safety check – should not happen */
                break;
            for (size_t k = 0; k < gen_len; ++k)
                gen_idx[k] = 0;
            break;
        }
        --pos;
    }
    return val;
}

/* sequence cache */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_cnt = 0;
static bool cache_init = false;

/* main sequence routine */
seq_t a(idx_t n)
{
    if (!cache_init) {
        ensure_capacity(8, &cache_cap, (void **)&cache, sizeof(seq_t));
        cache_init = true;
    }

    /* grow cache if needed */
    while (cache_cnt < (size_t)n) {
        seq_t cand = next_candidate();
        if (is_prime_big(cand)) {
            if (cache_cnt >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 8;
                ensure_capacity(new_cap, &cache_cap, (void **)&cache, sizeof(seq_t));
            }
            cache[cache_cnt++] = cand;
        }
    }
    return cache[n - 1];
}
