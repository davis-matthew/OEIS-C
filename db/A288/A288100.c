/*
 * A288100  Number of solutions to x^5 + y^5 = z^5 mod n.
 * https://oeis.org/A288100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) counts the triples (x,y,z) modulo n with x^5 + y^5 + z^5 ≡
 *            0 (mod n). Computed by brute force on the residue classes, with
 *            an O(n²) algorithm
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 */
    /* n fits safely into size_t for the intended tests */
    size_t N = (size_t)n;

    /* Allocate array f[a] = # of residues r with r^5 ≡ a (mod n) */
    seq_t *f = (seq_t *)malloc(N * sizeof(seq_t));
    if (!f) return 0;
    memset(f, 0, N * sizeof(seq_t));

    /* Count the fifth powers modulo n */
    for (size_t i = 0; i < N; ++i) {
        /* Compute i^5 mod n by repeated multiplication */
        uint64_t a1 = (uint64_t)i % (uint64_t)n;
        uint64_t a2 = (a1 * a1) % (uint64_t)n;       /* i^2 */
        uint64_t a3 = (a2 * a1) % (uint64_t)n;       /* i^3 */
        uint64_t a4 = (a3 * a1) % (uint64_t)n;       /* i^4 */
        uint64_t a5 = (a4 * a1) % (uint64_t)n;       /* i^5 */
        f[(size_t)a5] += 1;
    }

    /* Count triples (x,y,z) with x^5 + y^5 + z^5 ≡ 0 (mod n) */
    unsigned long long result = 0ULL;
    for (size_t i = 0; i < N; ++i) {
        if (f[i] == 0) continue;
        for (size_t j = 0; j < N; ++j) {
            if (f[j] == 0) continue;
            size_t sum = i + j;
            if (sum >= N) sum -= N;                     /* (i + j) mod n */
            size_t comp = (sum == 0) ? 0 : N - sum;      /* (-sum) mod n */
            if (f[comp] == 0) continue;
            /* Use uint64_t for intermediate multiplication to avoid
               overflow when seq_t is 64‑bit; the final result fits
               within 64 bits for n ≤ 10000. */
            result += (unsigned long long)f[i] *
                      (unsigned long long)f[j] *
                      (unsigned long long)f[comp];
        }
    }

    free(f);
    return (seq_t)result;
}
