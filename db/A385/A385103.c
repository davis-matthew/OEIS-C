/*
 * A385103  Number of values of s, 0 < s < n, such that -(s^s) == s (mod n).
 * https://oeis.org/A385103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A385103: a(n) = # { s | 0 < s < n and -(s^s) ≡ s (mod n) } */

/* This implementation loops over all s < n and counts those that satisfy the congruence
 * using the provided mod_pow helper. The algorithm runs in O(n log n) time, well within
 * the limits for n up to about 10000. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    /* n is at least 1 (offset = 1). For n = 1, no s exist, answer 0. */
    if (n <= 1)
        return 0;

    seq_t count = 0;
    seq_t mod = (seq_t)n;

    for (idx_t s = 1; s < n; ++s) {
        seq_t base = (seq_t)s;
        seq_t r = powmod(base, (uint64_t)s, mod);
        /* Check if r ≡ -base (mod mod)  ⇔  r + base ≡ 0 (mod mod) */
        if ((r + base) % mod == 0)
            ++count;
    }
    return count;
}
