/*
 * A294094  Sum of the differences of the larger and smaller parts in the
 *          partitions of 2n into two parts with the larger part prime and
 *          smaller part squarefree.
 * https://oeis.org/A294094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{x=1..n} 2*(n-x) for which (2n-x) is prime and x is
 *            squarefree. The function is O(n) and uses only the required
 *            arithmetic and bitwise operations
 * Range:     u8 n <= 35, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple primality test for 64‑bit values, fine for the range needed. */
static bool is_prime_uint64(uint64_t v)
{
    if (v < 2) return false;
    if (v % 2 == 0) return v == 2;
    if (v % 3 == 0) return v == 3;
    for (uint64_t i = 5; i * i <= v; i += 6)
        if (v % i == 0 || v % (i + 2) == 0)
            return false;
    return true;
}

/* Square‑free test for 64‑bit values. */
static bool is_squarefree_uint64(uint64_t x)
{
    for (uint64_t i = 2; i <= x / i; ++i) {
        uint64_t sq = i * i;
        if (x % sq == 0) return false;
    }
    return true;
}

/* Sequence A294094: Sum of the differences of the larger and
   smaller parts in the partitions of 2n into two parts with the
   larger part prime and the smaller part squarefree. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) return sum;          /* just in case */
    idx_t two_n = n + n;
    for (idx_t x = 1; x <= n; ++x) {
        idx_t prime_part = two_n - x;
        if (!is_prime_uint64(prime_part)) continue;
        if (!is_squarefree_uint64(x)) continue;
        seq_t diff = (seq_t)2 * ((seq_t)n - (seq_t)x);
        sum += diff;
    }
    return sum;
}
