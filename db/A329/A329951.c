/*
 * A329951  a(n) is the least prime k such that 2*n-1+k = 2*p*q for odd primes
 *          p,q (not necessarily distinct).
 * https://oeis.org/A329951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime k such that 2*n-1 + k = 2*p*q (with p and q
 *            odd primes, not necessarily distinct)
 * Range:     u8 n <= 195, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* Return the least prime k for the given index n. */
seq_t a(idx_t n)
{
    if (n == 0)               /* offset safety, not used by the OEIS sequence */
        return 0;

    /* base = 2*n - 1  (always odd) */
    seq_t base = ((seq_t)n) * (seq_t)2 - (seq_t)1;

    /* Enumerate odd primes k, starting with 3 */
    for (seq_t k = 3; ; k += 2) {
        if (!is_prime_small(k))
            continue;                /* skip non‑primes */

        /* product p*q = (base + k)/2  (even numerator divisible by 2) */
        seq_t m = (base + k) >> 1;   /* divide by 2 */

        /* For the expected range of n (≤ 10000) m fits comfortably in
           64‑bit unsigned, which we use for factorisation. */
        uint64_t m64 = (uint64_t)m;
        bool ok = false;

        for (uint64_t p = 3; p * p <= m64; p += 2) {
            if (m64 % p == 0) {
                uint64_t q = m64 / p;
                if (is_prime_small((seq_t)p) &&
                    is_prime_small((seq_t)q)) {
                    ok = true;
                    break;          /* found a valid factorisation */
                }
            }
        }

        if (ok)            /* k is the required least prime */
            return k;
    }
}
