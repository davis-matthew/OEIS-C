/*
 * A288105  Number of solutions to x^10 + y^10 = z^10 mod n.
 * https://oeis.org/A288105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = number of triples (x,y,z) in (ℤ/nℤ)^3 with x^10 + y^10 ≡
 *            z^10 (mod n). The function enumerates the residues of x^10
 *            modulo n, counts occurrences, then sums f(a)·f(b)·f((a+b) mod n)
 * Range:     u8 n <= 15, u16 n <= 99, u32 n <= 6143, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static inline uint64_t pow10_mod(uint64_t x, uint64_t mod) {
    uint64_t r = 1 % mod;
    for (int i = 0; i < 10; ++i)
        r = (r * x) % mod;
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Allocate array for residue counts */
    uint32_t *f = (uint32_t *)malloc((size_t)n * sizeof(uint32_t));
    if (!f) return 0;
    for (idx_t i = 0; i < n; ++i) f[i] = 0;

    /* Count x^10 residues */
    for (idx_t x = 0; x < n; ++x) {
        uint64_t r = pow10_mod(x, n);
        f[(size_t)r]++;
    }

    seq_t result = 0;
    for (idx_t a = 0; a < n; ++a) {
        uint32_t fa = f[(size_t)a];
        if (fa == 0) continue;
        for (idx_t b = 0; b < n; ++b) {
            uint32_t fb = f[(size_t)b];
            if (fb == 0) continue;
            idx_t c = a + b;
            if (c >= n) c -= n;
            uint32_t fc = f[(size_t)c];
            if (fc == 0) continue;
            result += (seq_t)fa * (seq_t)fb * (seq_t)fc;
        }
    }

    free(f);
    return result;
}
