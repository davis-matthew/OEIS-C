/*
 * A366982  a(n) is the smallest odd k > 1 such that n^((k+1)/2) == n (mod k).
 * https://oeis.org/A366982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest odd k > 1 such that n^((k+1)/2) ≡ n (mod k)
 * Range:     u8 n <= 766, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 102 terms through n = 9177 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;
    for (idx_t k = 3; k <= 1729; k += 2)
    {
        seq_t mod = (seq_t)k;
        seq_t n_mod = n_val % mod;
        seq_t powval = powmod(n_mod, (uint64_t)((k + 1) / 2), mod);
        if (powval == n_mod)
        {
            return mod;
        }
    }
    return (seq_t)1729;
}
