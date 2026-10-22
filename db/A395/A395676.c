/*
 * A395676  Numbers which are divisible by the sum of their base-6 digits.
 * https://oeis.org/A395676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 9228, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth number that is divisible by the sum of its base-6 digits
   (6-Harshad number).  The function performs a linear search, checking each
   integer in order until the nth valid number is found. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    idx_t candidate = 1;
    idx_t count = 0;

    for (;;) {
        /* Compute sum of base-6 digits */
        seq_t sum = 0;
        seq_t tmp = (seq_t)candidate;
        while (tmp) {
            sum += tmp % 6;
            tmp /= 6;
        }

        /* Candidate must be non‑zero, sum will be >0 automatically */
        if (candidate % sum == 0) {
            ++count;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        ++candidate;
    }
}
