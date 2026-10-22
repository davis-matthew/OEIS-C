/*
 * A324388  If n is a prime power (in A000961), then a(n) = n, otherwise a(n)
 *          is the greatest proper unitary divisor of n.
 * https://oeis.org/A324388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 9877, u32 n <= 9877, u64 n <= 9877,
 *            u128 n <= 9877, big any n
 * Helpers:   smallest_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A324388
   Definition:
   If n is a prime power (including 1), a(n) = n.
   Otherwise a(n) is the greatest proper unitary divisor of n.
   The greatest proper unitary divisor is n divided by the smallest
   prime‑power factor of n. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/smallest_prime_power.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;             /* 1 is treated as a prime power */

    seq_t sp = smallest_prime_power(n);

    /* If sp == n, n is a prime power */
    if (sp == (seq_t)n)
        return (seq_t)n;

    return (seq_t)n / sp;
}
