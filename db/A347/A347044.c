/*
 * A347044  Greatest divisor of n with half (rounded up) as many prime factors
 *          (counting multiplicity) as n.
 * https://oeis.org/A347044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the largest ⌈bigomega(n)/2⌉ prime factors
 *            (with multiplicity) of n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function finds the prime factorization of n, chooses the ⌈omega/2⌉ largest prime factors, and multiplies them. */

#include <stdint.h>
#include <stdlib.h>

/* Compare two uint64_t values for qsort in descending order. */
static int cmp_desc(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    if (x < y) return 1;
    if (x > y) return -1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t temp = (uint64_t)n;
    idx_t omega = 0;
    /* A 128‑slot array is more than enough for all n <= 2^64. */
    uint64_t factors[128];
    size_t fsize = 0;

    for (idx_t p = 2; p <= temp / p; ++p) {
        while (temp % (uint64_t)p == 0) {
            factors[fsize++] = (uint64_t)p;
            ++omega;
            temp /= (uint64_t)p;
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        factors[fsize++] = temp;
        ++omega;
    }

    idx_t target = (omega + 1) / 2;      /* ceil(omega/2) */

    /* Sort the prime factors in descending order. */
    qsort(factors, fsize, sizeof(uint64_t), cmp_desc);

    seq_t result = 1;
    for (size_t i = 0; i < (size_t)target && i < fsize; ++i) {
        result *= (seq_t)factors[i];
    }

    return result;
}
