/*
 * A264779  Numbers n for which the numbers 6n+1, 3n+2, 6n+7 are all odd
 *          composite squarefree, but not all semiprimes.
 * https://oeis.org/A264779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer m such that 6m+1, 3m+2, 6m+7 are all odd
 *            composite squarefree numbers, and not all three are semiprimes
 * Range:     u8 none, u16 n <= 774, u32 n <= 774, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   alloc_seq_array, count_distinct_primes, is_prime_big,
 *            is_squarefree, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/is_squarefree.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_realloc.h"

static bool good_candidate(idx_t m) {
    const seq_t a1 = ((seq_t)6 * m) + 1;
    const seq_t a2 = ((seq_t)3 * m) + 2;
    const seq_t a3 = ((seq_t)6 * m) + 7;

    /* 3m+2 must be odd; reject even m */
    if ((m & 1U) == 0)
        return false;

    const seq_t vals[3] = { a1, a2, a3 };
    bool all_semiprime = true;
    for (int i = 0; i < 3; ++i) {
        seq_t x = vals[i];
        /* must be odd, composite, and squarefree */
        if ((x & ((seq_t)1)) == 0 || x <= (seq_t)1 || is_prime_big(x))
            return false;
        if (!is_squarefree(x))
            return false;
        /* semiprime test: squarefree number with exactly two prime factors */
        uint64_t ux = (uint64_t)x;          /* fits in 64 bits for the required range */
        int pcount = count_distinct_primes(ux);
        if (pcount != 2)
            all_semiprime = false;
    }
    return !all_semiprime;   /* at least one must not be a semiprime */
}

static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t  last_tested = 0;

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;      /* undefined for n=0 – harmless */

    if (cache_len < (size_t)n) {
        size_t needed = n;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed)
                new_cap <<= 1;
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            if (!tmp)
                return (seq_t)0;   /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        for (idx_t m = last_tested + 1; ; ++m) {
            if ((m & 1U) == 0)
                continue;   /* 3m+2 would be even */
            if (good_candidate(m)) {
                cache[cache_len++] = (seq_t)m;
                if (cache_len >= (size_t)n) {
                    last_tested = m;
                    break;
                }
            }
            last_tested = m;
        }
    }

    return cache[n - 1];
}
