/*
 * A342844  Composite numbers not divisible by any of their nonzero digits.
 * https://oeis.org/A342844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Composite numbers not divisible by any of their nonzero digits.
 *            a(1)=27, a(2)=34, ... (see OEIS A342844)
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t n) */

/* Return the n-th composite number whose every non‑zero digit does NOT divide the number. */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* Undefined for n<1, return 0 as a guard */
    seq_t val = 4;                 /* smallest composite number */
    idx_t count = 0;

    for (;;) {
        /* Skip primes – they are not allowed, but only primes can be large */
        if (!is_prime_big(val)) {
            seq_t tmp = val;
            bool ok = true;
            while (tmp > 0) {
                seq_t digit = tmp % 10;
                if (digit != 0 && val % digit == 0) {
                    ok = false;
                    break;
                }
                tmp /= 10;
            }
            if (ok) {
                ++count;
                if (count == n) return val;
            }
        }
        ++val;
    }
}
