/*
 * A288101  Number of solutions to x^6 + y^6 = z^6 mod n.
 * https://oeis.org/A288101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = number of solutions to x^6 + y^6 ≡ z^6 (mod n)
 * Range:     u8 n <= 13, u16 n <= 111, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Each of x, y, z ranges over 0…n-1.  The implementation uses a */
/* straightforward O(n^2) convolution because n ≤ 10000 for the */
/* test set, which is well within the time limit.  */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not used, but safe for offset */
    size_t sz = (size_t)n;           /* safe length for allocations */

    /* Count how many x have each residue r = x^6 (mod n). */
    uint32_t *f = (uint32_t *)malloc(sz * sizeof(uint32_t));
    if (!f) return 0;                /* allocation failure */
    for (size_t i = 0; i < sz; ++i) f[i] = 0;

    for (idx_t x = 0; x < n; ++x) {
        seq_t r = 1;
        for (int i = 0; i < 6; ++i) r = (r * (seq_t)x) % n;
        f[(size_t)r] += 1;
    }

    /* Circular convolution h = f * f  (mod n).  */
    uint64_t *h = (uint64_t *)malloc(sz * sizeof(uint64_t));
    if (!h) { free(f); return 0; }
    for (size_t i = 0; i < sz; ++i) h[i] = 0;

    for (size_t i = 0; i < sz; ++i) {
        uint32_t fi = f[i];
        if (fi == 0) continue;
        for (size_t j = 0; j < sz; ++j) {
            uint32_t fj = f[j];
            if (fj == 0) continue;
            size_t s = (i + j) % sz;
            h[s] += (uint64_t)fi * fj;
        }
    }

    /* Total number of solutions = sum_s h[s] * f[s].  */
    uint64_t total = 0;
    for (size_t s = 0; s < sz; ++s) {
        total += h[s] * (uint64_t)f[s];
    }

    free(f);
    free(h);
    return (seq_t)total;
}
