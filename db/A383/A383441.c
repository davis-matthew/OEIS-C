/*
 * A383441  a(n) is the total of iterations needed in the binary GCD algorithm
 *          to compute gcd(n, k) for k = 0..n. The corresponding row of gcds
 *          is row n of A109004.
 * https://oeis.org/A383441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} (# of iterations of the * binary GCD
 *            algorithm (Knuth's Algorithm B) required to compute gcd(n,k))
 * Range:     u8 n <= 70, u16 n <= 7432, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9495 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count the number of loop iterations of Knuth's binary GCD (Algorithm B)
 * for the pair (a,b), where 0 < a,b.
 */
static inline seq_t bin_gcd_iterations(idx_t a, idx_t b)
{
    /* Strip common factors of 2.  a and b are >0 here. */
    uint64_t ua = a >> __builtin_ctzll(a);
    uint64_t ub = b >> __builtin_ctzll(b);
    seq_t cnt = 0;

    while (ua != ub) {
        /* Ensure ua <= ub. */
        if (ua > ub) {
            uint64_t tmp = ua;
            ua = ub;
            ub = tmp;
        }
        ub -= ua;
        /* Remove trailing zeros from ub. */
        ub >>= __builtin_ctzll(ub);
        ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    seq_t total = 0;

    /* For k = 0 the binary GCD terminates immediately, contributing 0. */
    for (idx_t k = 1; k <= n; ++k) {
        total += bin_gcd_iterations(n, k);
    }
    return total;
}
