/*
 * A309155  For integer n with prime factors p_i (1 <= i <= r), with
 *          repetition, (Omega(n) = r); a(n) = Sum_{i=1..r} k_i, where k_i is
 *          the least positive integer such that p_i - k_i | n - k_i.
 * https://oeis.org/A309155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 513, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 101 terms through n = 8177 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A309155: for each prime factor p of n (with multiplicity)
   add the least positive integer k such that (p-k) divides (n-k). */
#include <stdint.h>

static seq_t find_min_k(uint64_t p, uint64_t n)
{
    for (uint64_t k = 1; k < p; ++k) {
        int64_t d = (int64_t)p - (int64_t)k;                /* p - k   */
        if (d == 0) continue;                              /* skip divisor 0   */
        int64_t absd = d > 0 ? d : -d;                     /* |p-k|     */
        int64_t rem = (int64_t)n - (int64_t)k;             /* n-k   */
        if (rem % absd == 0) return (seq_t)k;              /* found minimal k */
    }
    return (seq_t)(p - 1);                                  /* fallback p-1   */
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;         /* a(1) = 0, and a(0) is not used          */
    uint64_t m = n;
    seq_t total = 0;

    /* factor 2 */
    uint64_t exp = 0;
    while ((m & 1ULL) == 0ULL) {
        ++exp;
        m >>= 1;
    }
    if (exp) total += find_min_k(2, n) * exp;

    /* factor odd primes */
    for (uint64_t i = 3; i * i <= m; i += 2) {
        if (m % i == 0) {
            exp = 0;
            while (m % i == 0) {
                ++exp;
                m /= i;
            }
            total += find_min_k(i, n) * exp;
        }
    }

    /* remaining prime > sqrt(m) */
    if (m > 1) {
        total += find_min_k(m, n);
    }

    return total;
}
