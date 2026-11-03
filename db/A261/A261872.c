/*
 * A261872  a(n) = phi(n) mod 5, where phi is the Euler totient function.
 * https://oeis.org/A261872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n) mod 5
 * Range:     u8 n <= 367, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9821 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/totient.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    return totient(n) % (seq_t)5;
}
