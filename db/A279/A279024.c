/*
 * A279024  Number of extradivisors of n (m < n is an extradivisor of n if for
 *          some positive k < n, m | n | k^(n+1) + m and n | (n-k)^(n+1) + m).
 * https://oeis.org/A279024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of m < n dividing n for which there exists a k (1
 *            ≤ k < n) such that k^(n+1) ≡ (n‑k)^(n+1) (mod n) and m = n –
 *            k^(n+1) (mod n). The function checks every k, collects the
 *            distinct admissible m, and returns their count. This is a direct
 *            search, not a closed formula
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/powmod64.h"   /* powmod64(uint64_t base, uint64_t exp, uint64_t mod) */

seq_t a(idx_t n)
{
    /* a(1) has no proper divisor */
    if (n < 2) return (seq_t)0;

    /* Use size_t for array sizing; for practical limits n ≤ SIZE_MAX here. */
    size_t N = (size_t)n;
    bool *used = (bool *)calloc(N, sizeof(bool));
    if (!used) return (seq_t)0;           /* allocation failure → 0 */

    for (size_t k = 1; k < N; ++k) {
        uint64_t r  = powmod64((uint64_t)k, n + 1u, N);
        if (r == 0) continue;              /* would give m = n, which is invalid */

        size_t k2 = N - k;
        uint64_t r2 = powmod64((uint64_t)k2, n + 1u, N);
        if (r != r2) continue;             /* residues differ → no suitable m */

        /* m = n - r (1 ≤ m < n) */
        size_t m = N - r;
        if (N % m == 0)                    /* m must divide n */
            used[m] = true;
    }

    /* Count distinct used m values */
    seq_t count = 0;
    for (size_t m = 1; m < N; ++m)
        if (used[m])
            count = count + (seq_t)1;

    free(used);
    return count;
}
