/*
 * A335437  Numbers k with a partition into two distinct parts (s,t) such that
 *          k | s*t.
 * https://oeis.org/A335437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A335437: Numbers k such that there exist distinct
 * s,t with s + t = k and k | s * t.
 *
 * A term k is valid iff the smallest integer l whose square is
 * divisible by k satisfies 2*l < k.  (Equivalently, l = A019554(k)
 * and l < k/2.)  The function enumerates k starting from 9, tests
 * this condition by factorising k, and returns the n-th such number.
 */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)0;        /* undefined for n < 1 */

    idx_t found = 0;
    for (seq_t k = 9; ; ++k) {
        /* Compute l = A019554(k) – minimal integer with l² divisible by k */
        seq_t l = (seq_t)1;
        idx_t temp = (idx_t)k;          /* use 64‑bit temp for factorisation */

        /* handle factor 2 separately */
        if (temp % 2 == 0) {
            int exp = 0;
            while (temp % 2 == 0) {
                temp /= 2;
                ++exp;
            }
            int ceil_exp = (exp + 1) / 2;
            l *= pow_seq((seq_t)2, ceil_exp);
        }

        /* odd prime factors */
        for (idx_t p = 3; p * p <= temp; p += 2) {
            if (temp % p == 0) {
                int exp = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                }
                int ceil_exp = (exp + 1) / 2;
                l *= pow_seq((seq_t)p, ceil_exp);
            }
        }

        /* remaining prime factor, if any */
        if (temp > 1) {
            l *= (seq_t)temp;          /* exponent is 1 → ceil_exp = 1 */
        }

        /* check the condition 2*l < k  (strictly < ensures distinct parts) */
        if ((seq_t)2 * l < k) {
            ++found;
            if (found == n)
                return k;
        }
    }
}
