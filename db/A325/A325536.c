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
 * Validated: partial, pass, 44 terms through n = 43 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper: compute Ω(m) for all m <= n using a smallest‑prime‑factor sieve. */
static void compute_omega_upto(idx_t n, unsigned *omega)
{
    if (n < 2)
        return;

    uint32_t *spf = (uint32_t*)calloc(n + 1, sizeof(uint32_t));
    for (idx_t i = 2; i * i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            for (idx_t j = i * i; j <= n; j += i) {
                if (spf[j] == 0)
                    spf[j] = (uint32_t)i;
            }
        }
    }
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0)
            spf[i] = (uint32_t)i;
    }

    omega[0] = 0;
    omega[1] = 0;
    for (idx_t x = 2; x <= n; ++x) {
        unsigned w = 0;
        uint32_t v = (uint32_t)x;
        while (v > 1) {
            ++w;
            v /= spf[v];
        }
        omega[x] = w;
    }

    free(spf);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Allocate arrays to store partition counts (dp) and cumulative omega sums (acc). */
    seq_t *dp   = mem_new(seq_t, (size_t)n + 1);
    seq_t *acc  = mem_new(seq_t, (size_t)n + 1);
    if (!dp || !acc) {
        /* In a real program one would handle allocation failures. */
        return 0;
    }

    /* Omega values for all part sizes. */
    unsigned *omega = (unsigned*)calloc((size_t)n + 1, sizeof(unsigned));
    if (!omega) {
        mem_free(dp);
        mem_free(acc);
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

    mem_free(dp);
    mem_free(acc);
    free(omega);
    return result;
}
