/*
 * A353179  a(n) is the first nonzero digit in the decimal expansion of
 *          1/prime(n).
 * https://oeis.org/A353179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the first non‑zero digit in the decimal expansion of
 *            1/prime(n)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    uint32_t p = get_prime(n);        /* nth prime, offset 1 */
    uint64_t rem = 1ULL;              /* remainder in long division */
    while (1) {
        rem *= 10ULL;                 /* bring next decimal digit */
        uint64_t digit = rem / p;     /* extract the digit */
        if (digit != 0ULL) {
            return (seq_t)digit;      /* first non‑zero digit found */
        }
        rem %= p;                     /* update the remainder */
    }
}
