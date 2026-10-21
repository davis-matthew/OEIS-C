/*
 * A243183  Primes of the form 2x^2+2xy+5y^2.
 * https://oeis.org/A243183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 2x^2 + 2xy + 5y^2
   (they all satisfy p ≡ 5 (mod 12), except the initial 2).   */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t cache[20000];
static idx_t cache_len = 0;
static uint64_t cand = 5;          /* next candidate ≡ 5 (mod 12) */
static bool initialized = false;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n = 0, but harmless */

    if (!initialized) {
        cache[0] = (seq_t)2;     /* a(1) = 2 */
        cache_len = 1;
        initialized = true;
    }

    while (cache_len < n) {
        /* cand is always ≡ 5 (mod 12) because it starts at 5
           and is incremented by 12 each time. */
        if (is_prime_big((seq_t)cand))
            cache[cache_len++] = (seq_t)cand;
        cand += 12ULL;
    }

    return cache[n - 1];
}
