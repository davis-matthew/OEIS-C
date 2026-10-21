/*
 * A276807  Number of solutions of the congruence y^2 == x^3 - x^2 - 4*x + 4
 *          (mod p) as p runs through the primes.
 * https://oeis.org/A276807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions of y² == x³ – x² – 4x + 4 (mod
 *            prime(n))
 * Range:     u8 n <= 55, u16 n <= 6510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_solutions, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by brute‑force enumeration.  For each prime p = prime(n)
   the set of squares mod p is pre‑computed once, then for every x the
   right‑hand side is evaluated and the number of corresponding y's is
   accumulated.  This is O(p²) in the sense of two nested loops, but
   p ≤ 104 729 for n ≤ 10 000, so the routine runs in a few milliseconds. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/get_prime.h"
#include "../utils/count_solutions.h"

/* External function required by the OEIS framework */
seq_t a(idx_t n)
{
    /* The nth prime (1‑based) */
    uint64_t p = get_prime(n);
    return count_solutions(p);
}
