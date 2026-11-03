/*
 * A270096  Smallest m such that 2^m == 2^n (mod n).
 * https://oeis.org/A270096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest m such that 2^m ≡ 2^n (mod n)
 * Range:     u8 n <= 874, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9224 (2026-11-01)
 * Helpers:   mulmod, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mulmod.h"
#include "../utils/powmod_u64.h"

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 0. */
    if (n == 1) {
        return (seq_t)0;
    }

    /* Compute r = 2^n mod n using 64‑bit arithmetic. */
    uint64_t mod = n;                /* n fits in uint64_t by definition of idx_t */
    uint64_t r   = powmod_u64(2ULL, n, mod);

    /* Iterate m until 2^m ≡ r (mod n). */
    uint64_t cur = 1ULL % mod;       /* 2^0 mod n */
    if (cur == r) {
        return (seq_t)0;
    }

    for (uint64_t m = 1; ; ++m) {
        cur = mulmod(cur, 2ULL, mod); /* next power of two modulo n */
        if (cur == r) {
            return (seq_t)m;
        }
    }
}
