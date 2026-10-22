/*
 * A362317  a(n) = n! * Sum_{k=0..floor(n/4)} (n/24)^k /(k! * (n-4*k)!).
 * https://oeis.org/A362317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/4)} (n/24)^k /(k! * (n-4*k)!)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Preemptively cast n to the sequence type for arithmetic. */
    const seq_t sn = (seq_t)n;

    /* The sum starts with the k = 0 term: always 1. */
    seq_t sum = (seq_t)1;

    /* binom_val = C(n, m) for current m in the loop, initialized to C(n,0) = 1. */
    seq_t binom_val = (seq_t)1;

    /* multcoef_val = (4k)! / (24^k * k!) for the current k, start at k = 0. */
    seq_t multcoef_val = (seq_t)1;

    /* n_power_val = n^k for the current k, start at k = 0 (i.e., 1). */
    seq_t n_power_val = (seq_t)1;

    /* Main loop: iterate m from 1 to n, updating binom(n,m). */
    for (idx_t m = (idx_t)1; m <= n; ++m) {
        /* Update binomial coefficient: C(n,m) = C(n,m-1) * (n-(m-1)) / m. */
        binom_val = (binom_val * (seq_t)(n - (m - (idx_t)1))) / (seq_t)m;

        /* Whenever m is a multiple of 4 we have a full term (k = m/4). */
        if ((m & (idx_t)3) == (idx_t)0) {  /* m % 4 == 0 */
            idx_t k = m >> 2;              /* k = m / 4 */

            /* n_power_val should be n^k. Multiply by n once to reach n^(k). */
            n_power_val *= sn;

            /* Compute next multcoef: (4k)!/(24^k * k!) recursively. */
            seq_t temp = multcoef_val;
            temp *= (seq_t)(4 * k - (idx_t)3);
            temp *= (seq_t)(4 * k - (idx_t)2);
            temp *= (seq_t)(4 * k - (idx_t)1);
            temp *= (seq_t)(4 * k);
            temp /= (seq_t)(24 * k);
            multcoef_val = temp;

            /* Term = binom_val * multcoef_val * n_power_val. */
            seq_t term = binom_val * multcoef_val;
            term *= n_power_val;

            /* Add to the running sum. */
            sum += term;
        }
    }

    return sum;
}
