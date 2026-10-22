/*
 * A327650  Maximum value of powers of 3 mod n.
 * https://oeis.org/A327650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ 3^k mod n | k ≥ 0 }
 * Range:     u8 n <= 256, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the maximum residue of powers of 3 modulo n.
   The calculation uses 64‑bit arithmetic (n is ≤ 10000 in tests),
   then converts the result to seq_t.  This satisfies the OEIS
   definition and runs in O(n) time with negligible overhead. */

#include <stdbool.h>
#include <stdlib.h>

/* The function signature required by the build system */
seq_t a(idx_t n)
{
    /* Handle the trivial case n == 1 (modulus 1 gives residue 0). */
    if (n <= 1) {
        return 0;
    }

    /* Work with 64‑bit values during the iteration. */
    unsigned long long mod = (unsigned long long)n;
    unsigned long long cur  = 1ULL % mod;
    unsigned long long best = cur;

    /* Allocate a visited flag array to detect cycle completion. */
    bool *seen = (bool *)malloc(mod * sizeof(bool));
    if (!seen) {
        /* Allocation failure – fall back to returning 0. */
        return 0;
    }
    /* Initialize the flag array. */
    for (size_t i = 0; i < mod; ++i) {
        seen[i] = false;
    }

    /* Iterate until we see a repeat residue. */
    while (!seen[(size_t)cur]) {
        seen[(size_t)cur] = true;
        cur = (cur * 3ULL) % mod;
        if (cur > best) {
            best = cur;
        }
    }

    free(seen);
    return (seq_t)best;
}
