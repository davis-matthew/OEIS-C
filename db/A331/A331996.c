/*
 * A331996  Number of Pythagorean triples mod n: total number of solutions
 *          (x,y,z) to x^2 + y^2 = z^2 mod n with x <= y.
 * https://oeis.org/A331996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions (x,y,z) to x^2 + y^2 ≡ z^2 (mod n)
 *            with x <= y. Computed by brute‑force search over all residue
 *            triples
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n == 0 safely (the sequence is defined for n ≥ 1). */
    if (n == 0)
        return (seq_t)0;

    /* Allocate arrays for z² mod n and counts of each residue. */
    idx_t *sq   = (idx_t *)malloc(n * sizeof(idx_t));
    idx_t *cnt  = (idx_t *)calloc(n, sizeof(idx_t));
    if (!sq || !cnt) {
        /* Allocation failed – clean up and return 0. */
        free(sq);
        free(cnt);
        return (seq_t)0;
    }

    /* Pre‑compute z² mod n for all z and count the occurrences. */
    for (idx_t z = 0; z < n; ++z) {
        unsigned __int128 prod = (unsigned __int128)z * z;
        idx_t r = (idx_t)(prod % n);
        sq[z] = r;
        cnt[r] += 1;
    }

    /* Count triples (x,y,z) with x <= y. */
    seq_t ans = 0;
    for (idx_t x = 0; x < n; ++x) {
        for (idx_t y = x; y < n; ++y) {
            unsigned __int128 sumsq = (unsigned __int128)sq[x] + sq[y];
            idx_t sum = (idx_t)(sumsq % n);
            ans += (seq_t)cnt[sum];
        }
    }

    /* Clean up. */
    free(sq);
    free(cnt);

    return ans;
}
