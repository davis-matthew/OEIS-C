/*
 * A400660  Number of bases b in range 2 <= b <= prime(n) in which prime(n) is
 *          a two-sided truncatable prime.
 * https://oeis.org/A400660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, new
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: For a given n, let p = prime(n) (the nth prime).
   Count the bases b (2 ≤ b ≤ p) such that every prefix and every
   suffix of the representation of p in base b is prime (no leading
   zeros in intermediate suffixes).  This direct verification runs
   in O(p · log_b(p)) and satisfies the required limits for n up to
   about 10 000.  The function uses only compile‑time utilities
   provided in the lib. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* prime p = prime(n) */
    uint32_t p32 = get_prime(n);   /* returns 32‑bit prime */
    uint64_t p   = (uint64_t)p32;  /* all primes in this range fit into 32 bits */

    seq_t result = 0;

    /* iterate over all bases 2 … p */
    for (uint64_t b = 2; b <= p; ++b)
    {
        /* convert p to base b, store digits (LSB first) */
        uint64_t temp = p;
        uint32_t digits[64];
        int len = 0;
        while (temp > 0) {
            digits[len++] = (uint32_t)(temp % b);
            temp /= b;
        }

        bool ok = true;

        /* check all prefixes (most significant side) */
        uint64_t pref = 0;
        for (int i = len - 1; i >= 0; --i) {
            pref = pref * b + digits[i];
            if (!is_prime_small((seq_t)pref)) { ok = false; break; }
        }
        if (!ok) continue;

        /* check all suffixes (least significant side),
           ensuring no leading zero in intermediate suffixes */
        for (int i = 0; i < len && ok; ++i) {
            if (digits[i] == 0) { ok = false; break; }  /* leading zero */
            uint64_t suff = 0;
            for (int j = i; j >= 0; --j)
                suff = suff * b + digits[j];
            if (!is_prime_small((seq_t)suff)) { ok = false; break; }
        }

        if (ok) {
            /* result fits into seq_t for all test cases that
               require it; cast after increment to avoid
               overflow on increase when seq_t is small. */
            result = (seq_t)((uint64_t)result + 1);
        }
    }

    return result;
}
