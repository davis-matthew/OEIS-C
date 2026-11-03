/*
 * A385729  Number of nonnegative values s < n such that (-s) == (-s)^s == s^s
 *          (mod n).
 * https://oeis.org/A385729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2008, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9886 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts number of s (< n) such that (-s) ≡ (-s)^s ≡ s^s (mod n). */
/* Brute‑force search – O(n·log n) time, works up to n ≈ 10 000. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety – offset starts at 1 */
    seq_t cnt = 0;
    seq_t mod_n = (seq_t)n;

    for (idx_t s = 0; s < n; ++s)
    {
        /* (-s) mod n == (n - s) mod n */
        seq_t alt_mod = ((seq_t)(n) - (seq_t)s) % mod_n;

        /* s^s mod n */
        seq_t pow_s = powmod((seq_t)s, s, mod_n);

        /* (-s)^s mod n */
        seq_t pow_alt = powmod(alt_mod, s, mod_n);

        if (pow_s == pow_alt && pow_s == alt_mod)
            ++cnt;
    }
    return cnt;
}
