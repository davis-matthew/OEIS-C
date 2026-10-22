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
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts number of s (< n) such that (-s) ≡ (-s)^s ≡ s^s (mod n). */
/* Brute‑force search – O(n·log n) time, works up to n ≈ 10 000. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

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
        seq_t pow_s = mod_pow((seq_t)s, s, mod_n);

        /* (-s)^s mod n */
        seq_t pow_alt = mod_pow(alt_mod, s, mod_n);

        if (pow_s == pow_alt && pow_s == alt_mod)
            ++cnt;
    }
    return cnt;
}
