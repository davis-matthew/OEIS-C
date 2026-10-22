/*
 * A366982  a(n) is the smallest odd k > 1 such that n^((k+1)/2) == n (mod k).
 * https://oeis.org/A366982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest odd k > 1 such that n^((k+1)/2) ≡ n (mod k)
 * Range:     u8 n <= 766, u16 n <= 9177, u32 n <= 9177, u64 n <= 9177,
 *            u128 n <= 9177, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;
    for (idx_t k = 3; k <= 1729; k += 2)
    {
        seq_t mod = (seq_t)k;
        seq_t n_mod = n_val % mod;
        seq_t powval = mod_pow(n_mod, (uint64_t)((k + 1) / 2), mod);
        if (powval == n_mod)
        {
            return mod;
        }
    }
    return (seq_t)1729;
}
