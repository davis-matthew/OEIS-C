/*
 * A296096  Numbers k with the property that k is the product of two distinct
 *          primes whose difference is also the product of two distinct
 *          primes.
 * https://oeis.org/A296096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 2703, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for numbers that are product of two distinct primes
 * and whose difference is also product of two distinct primes. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/factor_u64.h"

/* Return true iff n = p*q with p<q distinct primes and |p−q| = r*s
 * with r≠s distinct primes. */
static bool test_good(uint64_t n)
{
    if (n < 6)               /* 2*3 is the smallest product of two distinct primes */
        return false;

    uint64_t primes[8]  = {0};
    uint64_t exps[8]    = {0};
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    if (cnt != 2) return false;
    if (exps[0] != 1 || exps[1] != 1) return false;   /* distinct primes, no repeats */

    /* difference of the two primes */
    uint64_t diff = (primes[0] > primes[1]) ? primes[0] - primes[1] : primes[1] - primes[0];
    if (diff < 6) return false;   /* smallest product of two distinct primes is 6 */

    uint64_t pd[8] = {0};
    uint64_t ed[8] = {0};
    size_t cntd = 0;
    factor_u64(diff, pd, ed, &cntd);
    if (cntd != 2) return false;
    if (ed[0] != 1 || ed[1] != 1) return false;   /* distinct primes */

    return true;
}

/* Cached sequence values */
static seq_t *seq_cache      = NULL;   /* ptr to allocated sequence array */
static size_t seq_cap        = 0;      /* current capacity of seq_cache */
static size_t seq_len        = 0;      /* number of values currently stored */
static uint64_t last_tested  = 1;     /* all numbers <= last_tested have been examined */

seq_t a(idx_t n)
{
    if (n == 0)               /* safety for underflow */
        return 0;

    while (seq_len < n) {
        ++last_tested;
        if (test_good(last_tested)) {
            if (seq_len >= seq_cap) {
                seq_cap = seq_cap ? seq_cap << 1 : 8;
                seq_cache = (seq_t*)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)last_tested;
        }
    }
    return seq_cache[n - 1];
}
