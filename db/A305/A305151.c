/*
 * A305151  a(n) = (2n+1) - A294673(n), the amount by which A294673 is less
 *          than the maximum possible for n.
 * https://oeis.org/A305151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n+1) - A294673(n), where A294673(n) = smallest k>0 with
 *            (-2*(-1)^n)^k == +-1 (mod 4n+3)
 * Range:     u8 n <= 63, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 77 (2026-11-08)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"

static seq_t A294673_a(idx_t n)
{
    uint64_t M = (uint64_t)4 * (uint64_t)n + 3ULL;
    seq_t  M_seq = (seq_t)M;

    seq_t t_mod = (n & 1ULL) ? (seq_t)2 : (M_seq - (seq_t)2);

    uint64_t phi   = M;
    uint64_t mtmp  = M;
    for (uint64_t p = 2; p * p <= mtmp; ++p) {
        if (mtmp % p == 0) {
            phi = phi / p * (p - 1);
            while (mtmp % p == 0) mtmp /= p;
        }
    }
    if (mtmp > 1)
        phi = phi / mtmp * (mtmp - 1);

    uint64_t r = phi;
    uint64_t tempr = phi;
    for (uint64_t p = 2; p * p <= tempr; ++p) {
        if (tempr % p == 0) {
            while (r % p == 0) {
                uint64_t cand = r / p;
                if (powmod(t_mod, cand, M_seq) == (seq_t)1)
                    r = cand;
                else
                    break;
            }
            while (tempr % p == 0) tempr /= p;
        }
    }
    if (tempr > 1) {
        uint64_t p = tempr;
        while (r % p == 0) {
            uint64_t cand = r / p;
            if (powmod(t_mod, cand, M_seq) == (seq_t)1)
                r = cand;
            else
                break;
        }
    }

    if ((r & 1ULL) == 0) {
        seq_t mid = powmod(t_mod, r / 2, M_seq);
        if (mid == (seq_t)(M - 1))
            return (seq_t)(r / 2);
    }

    return (seq_t)r;
}

seq_t a(idx_t n)
{
    seq_t m = (seq_t)2 * (seq_t)n + (seq_t)1;
    return m - A294673_a(n);
}
