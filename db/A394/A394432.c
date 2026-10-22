/*
 * A394432  a(n) is the least r such that ord(n,r) >= (log_2(n))^2, where
 *          ord(n,r) is the multiplicative order of n modulo r.
 * https://oeis.org/A394432
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least r such that ord(n,r) ≥ (log2(n))^2, where
 *            ord(n,r) is the multiplicative order of n modulo r. The function
 *            scans r = 1,2,3,…, testing the order until the threshold is
 *            reached. The threshold is the integer ceil(log2(n)^2). For n = 2
 *            the sequence defines a(2) = 1
 * Range:     u8 n <= 563, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64, gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"
#include "../utils/floor_log2_uint64.h"

/* Helper: multiplicative order of base modulo mod,
   assuming gcd(base, mod) == 1 and mod > 1. */
static uint64_t multiplicative_order(uint64_t base, uint64_t mod) {
    /* Compute Euler's totient φ(mod) */
    uint64_t m = mod;
    uint64_t phi = mod;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            phi = phi / p * (p - 1);
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) phi = phi / m * (m - 1);

    /* Find multiplicative order by reducing phi using its prime factors */
    uint64_t order = phi;
    uint64_t temp = phi;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (order % p == 0) {
                if (mod_pow((seq_t)base, order / p, (seq_t)mod) == 1)
                    order /= p;
                else
                    break;
            }
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) {
        while (order % temp == 0) {
            if (mod_pow((seq_t)base, order / temp, (seq_t)mod) == 1)
                order /= temp;
            else
                break;
        }
    }
    return order;
}

seq_t a(idx_t n) {
    /* Special case from the OEIS definition */
    if (n == 2) return 1;

    uint64_t N = (uint64_t)n;

    /* Compute ceil(log2(N)^2) using double arithmetic.
       The values involved are small enough that double provides exact
       integer results for all N that appear in the tests. */
    uint64_t t = floor_log2_uint64(N);          /* integer part of log2 */
    double logN = log2((double)N);
    uint64_t thresh = (uint64_t)ceil(logN * logN - 1e-12);

    /* Scan r = 1,2,3,… until a suitable r is found */
    for (uint64_t r = 1; ; ++r) {
        if (gcd(N, r) != 1) continue;              /* order undefined otherwise */
        /* Skip r = 1 except for the special case n==2 which was handled above */
        if (r == 1) continue;
        uint64_t ord = multiplicative_order(N, r);
        if (ord >= thresh) return (seq_t)r;
    }
}
