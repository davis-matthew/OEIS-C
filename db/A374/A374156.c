/*
 * A374156  Primes p that are quadratic residues modulo 59. Row 17 of A373751.
 * https://oeis.org/A374156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   the nth prime p such that p is a quadratic residue modulo 59
 *            (i.e., Jacobi symbol (p|59) = 1, or p = 59)
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t count = 0;
    idx_t p = 1;          /* start from the prime after 1 */
    while (count < n) {
        ++p;
        if (!is_prime(p)) continue;
        if (p == 59) {
            ++count;
            continue;
        }
        if (jacobi_symbol((uint64_t)p, 59) == 1) {
            ++count;
        }
    }
    return (seq_t)p;
}
