/*
 * A362011  Numbers k such that 1 < gcd(k, 70) < k and A007947(k) does not
 *          divide 70.
 * https://oeis.org/A362011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 134, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9280 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A362011: numbers k such that 1 < gcd(k,70) < k and k has at least one
 * prime factor that does not divide 70.  Computed by a simple brute‑force
 * search. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

static bool has_other_prime(seq_t k)
{
    seq_t rem = k;
    seq_t p = (seq_t)2;
    /* trial division up to sqrt(k) */
    while (p * p <= rem) {
        if (rem % p == 0) {
            if (p != (seq_t)2 && p != (seq_t)5 && p != (seq_t)7)
                return true;
            while (rem % p == 0)
                rem /= p;
        }
        ++p;
    }
    /* remaining factor, if any */
    if (rem > 1) {
        if (rem != (seq_t)2 && rem != (seq_t)5 && rem != (seq_t)7)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* The sequence is zero‑based in OEIS, but the problem statement says
     * a(1) is the first term. Therefore we look for the nth term with
     * n>=1. */
    seq_t target = (seq_t)n;
    idx_t count = 0;
    seq_t candidate = 0;
    seq_t seventy = (seq_t)70;
    seq_t k = 2;
    while (1) {
        seq_t kk = (seq_t)k;
        /* check 1 < gcd(k,70) < k */
        seq_t g = gcd(kk, seventy);
        if (g > 1) {
            if (has_other_prime(kk)) {
                ++count;
                if (count == target)
                    return kk;
            }
        }
        ++k;
    }
}
