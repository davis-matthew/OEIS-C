/*
 * A380536  a(n) = 1 if n is a multiple of A351566(n), otherwise 0, where
 *          A351566 is the radix prime of the second least significant nonzero
 *          digit in the primorial base expansion of n, or 1 if there is no
 *          such digit.
 * https://oeis.org/A380536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a multiple of A351566(n), otherwise 0;
 *            A351566(n) is the radix prime of the second least significant
 *            non‑zero digit in the primorial base expansion of n, or 1 if
 *            there is no such digit
 * Range:     u8 n <= 3447, u16 n <= 9531, u32 n <= 9531, u64 n <= 9531,
 *            u128 n <= 9531, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* first few primes – enough to cover all bits of a 64‑bit integer. */
    static const uint32_t primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
        73, 79, 83, 89, 97, 101, 103, 107, 109, 113
    };
    const size_t primes_count = sizeof(primes) / sizeof(primes[0]);

    uint64_t rem = (uint64_t)n;     /* work with a native 64‑bit value */
    int nonzero = 0;
    seq_t radPrime = 1;            /* default when there is no second digit */

    for (size_t i = 0; i < primes_count && rem > 0; ++i) {
        uint32_t p   = primes[i];
        uint32_t dig = rem % p;
        rem /= p;                 /* shift to the next higher digit */
        if (dig != 0) {
            ++nonzero;
            if (nonzero == 2) {
                radPrime = (seq_t)p;   /* second non‑zero digit found */
                break;
            }
        }
    }
    /* If fewer than two non‑zero digits were encountered, radPrime stays 1 */
    seq_t mod = (seq_t)n % radPrime;
    return (mod == 0) ? (seq_t)1 : (seq_t)0;
}
