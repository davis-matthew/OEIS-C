/*
 * A337198  Number of distinct prime factors in A337194(n) =
 *          1+A000265(sigma(n)), where A000265(k) gives the odd part of k.
 * https://oeis.org/A337198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime factors of (1 +
 *            odd_part(sigma(n)))
 * Range:     u8 n <= 984, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9797 (2026-11-01)
 * Helpers:   odd_part, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is A337198. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/odd_part.h"
#include "../utils/omega.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Compute sigma(n) */
    seq_t s = sigma_of(n);
    /* Compute odd part of sigma(n) */
    seq_t odd_s = odd_part(s);
    /* Compute t = 1 + odd_part(sigma(n)) */
    seq_t t = odd_s + 1;
    /* Count distinct prime factors of t (t fits in 64 bits for the intended range) */
    uint64_t t64 = (uint64_t)t;
    seq_t res = (seq_t)omega(t64);
    return res;
}
