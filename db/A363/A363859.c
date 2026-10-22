/*
 * A363859  Number of divisors of 7*n-1 of the form 7*k+2.
 * https://oeis.org/A363859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of 7*n‑1 such that d ≡ 2 (mod 7)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   combine_prime, count_mod7_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/combine_prime.h"
#include "../utils/count_mod7_two.h"

seq_t a(idx_t n)
{
    /* 7*n-1 is at most 64‑bit for all reasonable n used in tests.
       We compute it in 64 bit for factorization. */
    uint64_t M = 7 * n - 1;
    return count_mod7_two(M);
}
