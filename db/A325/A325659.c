/*
 * A325659  Smallest Brazilian composite in base n >= 2 which can be
 *          represented as a string of three or more 1's in this base.
 * https://oeis.org/A325659
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 797, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the smallest composite repunit in base n (length ≥ 3)
   that is also a repdigit in some base other than n. */
#include <stdbool.h>
#include <stdint.h>

/* Utility functions from utils (assumed to be available). */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n) */
#include "../utils/big_pow.h"       /* seq_t big_pow(seq_t base, seq_t exp) */

static bool is_brazilian_other_base(seq_t v, idx_t base_n)
{
    /* Treat v as 64‑bit for divisor enumeration.  The values that appear
       for n ≤ 10000 fit comfortably in 64 bits. */
    uint64_t val = (uint64_t)v;
    /* length‑2 case: v = d * (b+1)  →  b+1 divides v */
    for (uint64_t d1 = 1; d1 * d1 <= val; ++d1) {
        if (val % d1) continue;
        uint64_t d2 = val / d1;
        /* check the divisor pair d1 */
        if (d1 > 2) {
            uint64_t b = d1 - 1;
            if (b != (uint64_t)base_n) {
                uint64_t d = val / d1;        /* digit */
                if (d < b) return true;
            }
        }
        /* check the counterpart d2 when distinct */
        if (d2 != d1 && d2 > 2) {
            uint64_t b = d2 - 1;
            if (b != (uint64_t)base_n) {
                uint64_t d = val / d2;
                if (d < b) return true;
            }
        }
    }

    /* length‑≥ 3 case */
    for (idx_t m = 3; ; ++m) {
        /* if even the minimal base b = 2 gives a Geometric Sum > v,
           then all larger bases will also be too large → stop */
        seq_t pow_two_m  = big_pow((seq_t)2, (seq_t)m);
        seq_t min_sum_b2 = pow_two_m - (seq_t)1;   /* 2^m – 1 */
        if (min_sum_b2 > (seq_t)val) break;

        for (uint64_t b = 2; ; ++b) {
            /* Incrementally compute S = 1 + b + b^2 + … + b^(m‑1) */
            seq_t S = (seq_t)1;
            seq_t term = (seq_t)1;
            bool overflow = false;
            for (idx_t i = 1; i < m; ++i) {
                /* term = b^i */
                if (term > ((seq_t)UINT64_MAX / (seq_t)b)) {
                    overflow = true; break;
                }
                term *= (seq_t)b;
                S += term;
                if (S > (seq_t)val) break;
            }
            if (overflow || S > (seq_t)val) break;  /* larger b ⇒ larger S */

            if (S == 0) break;   /* avoid division by zero */

            if ((seq_t)val % S == (seq_t)0) {
                seq_t d = (seq_t)val / S;
                if (d < (seq_t)b && (uint64_t)b != (uint64_t)base_n) {
                    return true;
                }
            }
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    for (idx_t k = 3; ; ++k) {
        seq_t pow_nk = big_pow((seq_t)n, (seq_t)k);
        seq_t repunit = (pow_nk - (seq_t)1) / ((seq_t)n - (seq_t)1);
        /* Skip prime repunits */
        if (is_prime_big(repunit)) continue;
        if (is_brazilian_other_base(repunit, n)) return repunit;
    }
}
