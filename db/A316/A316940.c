/*
 * A316940  Smallest "anti-Carmichael pseudoprime" to base n.
 * https://oeis.org/A316940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the smallest k such that
   n^(k‑1) ≡ 1 (mod k) and for every prime p dividing k
   (p‑1) does NOT divide (k‑1).  It searches k sequentially
   starting from 2 and tests the two conditions directly. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* undefined for n = 0 */
    seq_t nseq = (seq_t)n;             /* base for modular powers */

    for (uint64_t k = 2;; ++k) {
        seq_t kseq = (seq_t)k;

        /* Condition 1: n^(k-1) ≡ 1 (mod k) */
        if (mod_pow(nseq, k - 1, kseq) != (seq_t)1)
            continue;

        /* Condition 2: for all prime factors p of k,
           (p-1) does NOT divide (k-1) */
        uint64_t km1   = k - 1;
        uint64_t temp  = k;
        bool      ok   = true;

        for (uint64_t p = 2; p <= temp / p; ++p) {
            if (temp % p == 0) {
                if (km1 % (p - 1) == 0) {
                    ok = false;
                    break;
                }
                while (temp % p == 0)
                    temp /= p;
            }
        }
        if (ok && temp > 1) {
            /* temp is now a prime factor > sqrt(original k) */
            if (km1 % (temp - 1) == 0)
                ok = false;
        }

        if (ok)
            return kseq;
    }
}
