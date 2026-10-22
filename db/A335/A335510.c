/*
 * A335510  Number of (1,1,1)-matching permutations of the prime indices of n.
 * https://oeis.org/A335510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   factorial_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A335510: Number of (1,1,1)-matching permutations of the prime indices of n.
   Formula: a(n) = 0 if n is cubefree; otherwise a(n) = (Σ e_i)! / Π e_i!,
   where e_i are the exponents in the prime factorization of n. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factorial_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* n ≥ 1 by specification */
    uint64_t m = (uint64_t)n;
    /* At most log2(n) distinct primes for n ≤ 2^64, allocate on stack */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;

    uint64_t tmp = m;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        primes[pcnt] = tmp;
        exps[pcnt] = 1;
        ++pcnt;
    }

    /* Check for cubefree: all exponents <= 2 → result 0 */
    for (size_t i = 0; i < pcnt; ++i) {
        if (exps[i] >= 3) goto not_cubefree;
    }
    return (seq_t)0;

not_cubefree:
    /* Sum of all exponents */
    uint64_t sum_e = 0;
    for (size_t i = 0; i < pcnt; ++i) sum_e += exps[i];

    /* Compute multinomial coefficient: sum_e! / Π exps! */
    seq_t result = factorial_uint64(sum_e);
    for (size_t i = 0; i < pcnt; ++i) {
        result /= factorial_uint64(exps[i]);  /* exact division */
    }
    return result;
}
