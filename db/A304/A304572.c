/*
 * A304572  Triangle read by rows: T(n,k) = 1 if k does not divide n^e,
 *          positive nonzero integers, and gcd(n,k) > 1.
 * https://oeis.org/A304572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9816, u16 n <= 9816, u32 n <= 9816, u64 n <= 9816,
 *            u128 n <= 9816, big any n
 * Helpers:   tri_row_from_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A304572: triangle read by rows.
   Returns 0 or 1 for the nth entry defined by:
   T(n,k)=1 iff gcd(n,k)>1 AND k has at least one prime factor not dividing n.
   The nth term is the entry read in row-major order. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/tri_row_from_index.h"

/* Primes up to 141 (maximum row/column index for n <= 10000). */
static const uint32_t primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
    53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107,
    109, 113, 127, 131, 137, 139
};
static const size_t prime_count = sizeof(primes) / sizeof(primes[0]);

/* Return index of prime p in the array, or prime_count if not found. */
static size_t prime_index(uint32_t p)
{
    for (size_t i = 0; i < prime_count; ++i)
        if (primes[i] == p) return i;
    return prime_count;   /* not found – should not happen for our range */
}

/* Return a bitmask of the distinct prime divisors of x. */
static uint64_t prime_mask(uint32_t x)
{
    uint64_t mask = 0;
    uint32_t temp = x;
    for (size_t i = 0; i < prime_count; ++i) {
        if (primes[i] * primes[i] > temp) break;
        if (temp % primes[i] == 0) {
            mask |= (uint64_t)1 << i;
            while (temp % primes[i] == 0) temp /= primes[i];
        }
    }
    if (temp > 1) {            /* remaining prime factor > sqrt(original x) */
        size_t idx = prime_index(temp);
        if (idx < prime_count) mask |= (uint64_t)1 << idx;
    }
    return mask;
}

/* True if the triangle entry T(row,col) equals 1. */
static int triangle_entry_one(uint32_t row, uint32_t col)
{
    uint64_t mask_row = prime_mask(row);
    uint64_t mask_col = prime_mask(col);
    int has_common = (mask_row & mask_col) != 0;
    int has_non     = (~mask_row & mask_col) != 0;
    return has_common && has_non;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    idx_t row = tri_row_from_index(n);
    idx_t prev_total = (row - 1) * row / 2;
    idx_t col = n - prev_total;
    return triangle_entry_one((uint32_t)row, (uint32_t)col) ? (seq_t)1 : (seq_t)0;
}
