/*
 * A325536  Sum of sums of omegas of parts over all integer partitions of n.
 * https://oeis.org/A325536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{partitions p of n} Σ_{part in p} ω(part) where ω(m) is
 *            the number of prime factors of m counted with multiplicity. The
 *            implementation uses a dynamic programming approach that
 *            simultaneously counts the partitions and accumulates the total
 *            omega contribution. It has O(n²) time and O(n) space
 * Range:     u8 n <= 11, u16 n <= 31, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   compute_omega_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/compute_omega_upto.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Allocate arrays to store partition counts (dp) and cumulative omega sums (acc). */
    seq_t *dp   = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *acc  = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp || !acc) {
        /* In a real program one would handle allocation failures. */
        return 0;
    }

    /* Omega values for all part sizes. */
    unsigned *omega = (unsigned*)calloc((size_t)n + 1, sizeof(unsigned));
    if (!omega) {
        free(dp);
        free(acc);
        return 0;
    }
    compute_omega_upto(n, omega);

    dp[0]  = 1;        /* One way to partition 0: the empty partition. */
    acc[0] = 0;        /* No omega contribution for the empty partition. */

    for (idx_t part = 1; part <= n; ++part) {
        unsigned w = omega[part];
        for (idx_t sum = part; sum <= n; ++sum) {
            seq_t temp = dp[sum - part] * (seq_t)w;   /* New omega from adding this part. */
            acc[sum] += acc[sum - part] + temp;       /* Add old omega + new part's omega. */
            dp[sum]   += dp[sum - part];               /* Update partition count. */
        }
    }

    seq_t result = acc[n];

    free(dp);
    free(acc);
    free(omega);
    return result;
}
