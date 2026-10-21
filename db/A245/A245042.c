/*
 * A245042  Primes of the form (k^2+4)/5.
 * https://oeis.org/A245042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes p such that 5*p-4 is a perfect square
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 57, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset starts at 1 */

    /* Static cache of generated primes */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_m = 1;        /* next m to examine (k = 5m±1) */

    /* Generate terms until we have at least n entries */
    while (cache_len < n) {
        seq_t m = (seq_t)next_m;
        seq_t m_sq = m * m;

        /* Candidates in order of increasing k */
        seq_t p_minus = m_sq * 5 - m * 2 + 1;   /* k = 5m-1 */
        seq_t p_plus  = m_sq * 5 + m * 2 + 1;   /* k = 5m+1 */

        if (p_minus >= 2 && is_prime_big(p_minus)) {
            if (cache_len == cache_cap)
                grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = p_minus;
        }
        if (p_plus >= 2 && is_prime_big(p_plus)) {
            if (cache_len == cache_cap)
                grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = p_plus;
        }

        next_m++;   /* advance to next m for subsequent calls */
    }

    return cache[n - 1];
}
