/*
 * A257993  Least gap in the partition having Heinz number n; index of the
 *          least prime not dividing n.
 * https://oeis.org/A257993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the first prime that does not divide n
 *            (equivalently, the least positive integer not a part of the
 *            partition whose Heinz number is n)
 * Range:     u8 n <= 8980, u16 n <= 8980, u32 n <= 8980, u64 n <= 8980,
 *            u128 n <= 8980, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* primes up to 53 are sufficient for all 64‑bit n:
       the product of the first 15 primes (47#) fits in 64 bits,
       and adding the next prime (53) would exceed the limit,
       so some prime among these is always missing from any n. */
    static const uint64_t primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19,
        23, 29, 31, 37, 41, 43, 47, 53
    };
    const size_t prime_count = sizeof(primes) / sizeof(primes[0]);

    for (size_t i = 0; i < prime_count; ++i) {
        if ((n % primes[i]) != 0ULL) {
            /* indices are 1‑based for the sequence */
            return (seq_t)(i + 1);
        }
    }
    /* In theory this line is unreachable for 64‑bit inputs. */
    return (seq_t)(prime_count + 1);
}
