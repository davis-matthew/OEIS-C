/*
 * A316990  Smallest exponent m of n such that A289280(n) | n^m.
 * https://oeis.org/A316990
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the smallest exponent m such that the smallest
   integer k > n with k | n^e (e > 1) divides n^m.  The integer
   k is the smallest S‑smooth number > n, where S is the set of
   distinct prime divisors of n.  */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* defined only for n ≥ 2 */

    /* factor n : primes_n[0..pcnt-1] with exponents ex_n[0..pcnt-1] */
    uint64_t primes_n[64];
    uint64_t ex_n[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes_n, ex_n, &pcnt);

    /* find the smallest S‑smooth integer k > n */
    uint64_t k = 0;
    for (uint64_t cand = n + 1; ; ++cand) {
        uint64_t primes_c[64];
        uint64_t ex_c[64];
        size_t ccnt = 0;
        factor_u64(cand, primes_c, ex_c, &ccnt);

        bool smooth = true;
        for (size_t i = 0; i < ccnt && smooth; ++i) {
            bool present = false;
            for (size_t j = 0; j < pcnt; ++j) {
                if (primes_c[i] == primes_n[j]) { present = true; break; }
            }
            if (!present) smooth = false;
        }
        if (smooth) { k = cand; break; }
    }

    /* factor k to determine the necessary exponent m */
    uint64_t primes_k[64];
    uint64_t ex_k[64];
    size_t kcnt = 0;
    factor_u64(k, primes_k, ex_k, &kcnt);

    size_t m = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes_n[i];
        size_t exp_k = 0;
        for (size_t j = 0; j < kcnt; ++j) {
            if (primes_k[j] == p) { exp_k = ex_k[j]; break; }
        }
        size_t exp_n = ex_n[i];
        size_t need = (exp_k + exp_n - 1) / exp_n;   /* ceil division */
        if (need > m) m = need;
    }

    if (m < 2) m = 2;   /* for safety; rarely needed */
    return (seq_t)m;
}
