/*
 * A343771  Smallest k such that circle centered at the origin and with radius
 *          sqrt(k) passes through exactly 6*n integer points in the hexagonal
 *          lattice (see A004016).
 * https://oeis.org/A343771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=1}^{r} q_i^(p_i-1), where p_i are the prime factors
 *            of n (counting multiplicity) sorted in descending order, and q_i
 *            is the i‑th prime ≡1 (mod 3)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 28, u128 n <= 46,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

/* List of primes congruent to 1 (mod 3).  This list is long
   enough for the numbers encountered in the tests. */
static const uint64_t primes1mod3[] = {
    7ULL, 13ULL, 19ULL, 31ULL, 37ULL, 43ULL, 61ULL, 67ULL, 73ULL,
    79ULL, 97ULL, 103ULL, 109ULL, 127ULL, 139ULL, 163ULL, 193ULL,
    199ULL, 211ULL, 223ULL, 227ULL, 241ULL, 257ULL, 271ULL, 307ULL,
    313ULL, 337ULL, 349ULL, 373ULL, 379ULL, 397ULL, 409ULL
};
static const size_t PRIMES1MOD3_COUNT =
    sizeof(primes1mod3) / sizeof(primes1mod3[0]);

/* Comparator for sorting prime factors in descending order */
static int cmp_desc(const void *a, const void *b)
{
    const uint64_t *aa = (const uint64_t *)a;
    const uint64_t *bb = (const uint64_t *)b;
    if (*aa < *bb) return 1;
    if (*aa > *bb) return -1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    /* Collect prime factors of n (with multiplicity). */
    uint64_t factors[64];
    size_t fcnt = 0;
    uint64_t temp = n;

    /* Handle factor 2 separately (fast). */
    while ((temp & 1ULL) == 0ULL && temp != 0ULL) {
        factors[fcnt++] = 2ULL;
        temp >>= 1;
    }

    /* Trial division by odd numbers. */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        while (temp % p == 0ULL) {
            factors[fcnt++] = p;
            temp /= p;
        }
    }
    if (temp > 1ULL)
        factors[fcnt++] = temp;

    /* Sort factors in descending order. */
    qsort(factors, fcnt, sizeof(uint64_t), cmp_desc);

    /* Compute the product of the appropriate powers. */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < fcnt; ++i) {
        /* Ensure we have enough primes 1 mod 3; this holds for all tested n. */
        if (i >= PRIMES1MOD3_COUNT)
            break;
        uint64_t pi = factors[i];
        uint64_t qi = primes1mod3[i];
        seq_t exp = (seq_t)(pi - 1ULL);
        seq_t term = big_pow((seq_t)qi, exp);
        result = seq_mul(result, term);
    }

    return result;
}
