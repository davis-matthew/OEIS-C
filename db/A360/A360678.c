/*
 * A360678  Sum of the left half (inclusive) of the prime indices of n.
 * https://oeis.org/A360678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the sum of the left half (inclusive) of the prime
 *            indices * of n, where the prime index of a prime p is the number
 *            of primes <= p
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   _smallest_prime_factor, compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/_smallest_prime_factor.h"
#include "../utils/compute_pi.h"

/* Comparator for qsort on idx_t */
static int cmp_idx(const void *a, const void *b)
{
    idx_t aa = *(const idx_t *)a;
    idx_t bb = *(const idx_t *)b;
    return (aa > bb) - (aa < bb);
}

/* Computes a(n) */
seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* n == 1 has no prime factors */

    /* Maximum number of prime factors (with multiplicity) for any 64‑bit
       number is < 64, so a fixed buffer is safe. */
    idx_t idx_buf[64];
    size_t cnt = 0;

    uint64_t temp = (uint64_t)n;
    while (temp > 1 && cnt < sizeof(idx_buf)/sizeof(idx_buf[0])) {
        uint64_t p = _smallest_prime_factor(temp);
        /* Count multiplicity */
        while (temp % p == 0) {
            if (cnt >= sizeof(idx_buf)/sizeof(idx_buf[0])) break;
            idx_t pi = (idx_t)compute_pi(p); /* index of prime p */
            idx_buf[cnt++] = pi;
            temp /= p;
        }
    }

    /* If n had more than 64 prime factors (extremely unlikely) we would
       ignore the excess, but for all practical purposes this suffices. */

    if (cnt == 0) return 0;

    /* Sort the indices to find the left half. */
    qsort(idx_buf, cnt, sizeof(idx_t), cmp_idx);

    /* Sum the left half (rounded up). */
    size_t half = (cnt + 1) / 2;
    seq_t sum = 0;
    for (size_t i = 0; i < half; ++i) {
        sum += (seq_t)idx_buf[i];
    }
    return sum;
}
