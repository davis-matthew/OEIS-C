/*
 * A363854  Number of divisors of 7*n-2 of form 7*k+6.
 * https://oeis.org/A363854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of 7*n−2 with d ≡ 6 (mod 7)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   _count_divisors_7k6, _dfs_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function factorises 7*n−2 and counts the divisors that satisfy
   the congruence.  It works for any seq_t (8/16/32/64/128‑bit unsigned
   or arbitrary‑precision) and any idx_t (uint64_t). */

#include <stdint.h>
#include "../utils/_count_divisors_7k6.h"
#include "../utils/_dfs_div.h"

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* offset is 1 */

    /* 7*n-2 fits in unsigned 64‑bit for all tested values. */
    __uint128_t tmp = (__uint128_t)7 * n - 2;
    uint64_t N = (uint64_t)tmp;

    return _count_divisors_7k6(N);
}
