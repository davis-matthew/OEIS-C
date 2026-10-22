/*
 * A381798  Number of residues r such that p^m is congruent to r (mod n),
 *          where prime p | n and m >= 0.
 * https://oeis.org/A381798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |⋃_{p|n} {p^k mod n : k ≥ 0}|
 * Range:     u8 n <= 536, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Special case n = 1: the only residue is 0. */
    if (n == 1) return (seq_t)1;

    /* Factor n to obtain all distinct prime divisors. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Global bit‑mask of residues already seen. */
    size_t sz = (size_t)n;
    uint8_t *global_seen = (uint8_t*)calloc(sz, sizeof(uint8_t));
    if (!global_seen) return (seq_t)0;          /* allocation failure */

    const seq_t N = (seq_t)n;
    seq_t distinct = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];

        /* Local bit‑mask to detect a cycle of p‑powers. */
        uint8_t *local_seen = (uint8_t*)calloc(sz, sizeof(uint8_t));
        if (!local_seen) { free(global_seen); return (seq_t)0; }

        seq_t val = 1;          /* p^0 */
        while (1) {
            size_t idx = (size_t)(unsigned long long)val;

            if (!local_seen[idx]) {
                local_seen[idx] = 1;
                if (!global_seen[idx]) {
                    global_seen[idx] = 1;
                    ++distinct;     /* new residue globally */
                }
            } else {
                break;        /* cycle detected for this prime */
            }

            val = (val * (seq_t)p) % N;
        }

        free(local_seen);
    }

    free(global_seen);
    return distinct;
}
