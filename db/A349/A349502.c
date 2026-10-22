/*
 * A349502  Numbers k such that the continued fraction of the harmonic mean of
 *          the divisors of k contains distinct elements.
 * https://oeis.org/A349502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1032, u32 n <= 9436, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:
   Generate integers k in increasing order.  For each k compute
   the harmonic mean of its divisors HM = k * τ(k) / σ(k), where
   τ(k) is the number of divisors and σ(k) the sum of divisors.
   Reduce HM to lowest terms p/q and obtain its simple continued
   fraction by the Euclidean algorithm.  Accept k iff all
   partial quotients are distinct.  The function returns the
   nth accepted integer (1‑based index). */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* static state */
    static bool init = false;
    static uint64_t last_checked = 0;
    static seq_t *seq_arr = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;

    if (!init) {
        seq_cap = 256;
        seq_arr = seq_alloc(seq_cap);
        seq_len = 0;
        init = true;
    }

    /* Generate until we have at least n terms */
    while (seq_len < n) {
        uint64_t k = last_checked + 1;
        last_checked = k;

        /* factorization */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt;
        factor_u64(k, primes, exps, &cnt);

        /* τ(k) */
        seq_t tau = 1;
        for (size_t i = 0; i < cnt; ++i) {
            tau *= (seq_t)(exps[i] + 1);
        }

        /* σ(k) */
        seq_t sigma = 1;
        for (size_t i = 0; i < cnt; ++i) {
            seq_t p = (seq_t)primes[i];
            seq_t pow = 1;
            for (uint64_t j = 0; j < exps[i] + 1; ++j) {
                pow *= p;
            }
            seq_t term = (pow - 1) / (p - 1);
            sigma *= term;
        }

        /* HM = k * tau / sigma, reduced */
        seq_t num = (seq_t)k * tau;
        seq_t den = sigma;
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* continued fraction with distinct quotients */
        bool distinct = true;
        seq_t p = num, q = den;
        seq_t quot[64];
        size_t qlen = 0;

        while (q != 0 && distinct) {
            seq_t a0 = p / q;
            for (size_t i = 0; i < qlen; ++i) {
                if (quot[i] == a0) {
                    distinct = false;
                    break;
                }
            }
            if (!distinct) break;

            quot[qlen++] = a0;
            seq_t r = p % q;
            p = q;
            q = r;
        }

        if (distinct) {
            if (seq_len == seq_cap) {
                size_t old_cap = seq_cap;
                seq_cap *= 2;
                seq_arr = seq_realloc(seq_arr, old_cap, seq_cap);
            }
            seq_arr[seq_len++] = (seq_t)k;
        }
    }

    return seq_arr[n - 1];
}
