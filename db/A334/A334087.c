/*
 * A334087  Draw the lines with equations y=kx (k=1..n) on the R^2/Z^2 square
 *          flat torus. a(n) is the number of intersection points.
 * https://oeis.org/A334087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{q=1}^{n-1} φ(q) * f(n,q) where φ is Euler's totient
 *            and f(n,q) = 1_{n≥2q} + min(q-1, n-q)_{n>q}
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Maximum denominator */
    size_t max_q = (size_t)(n - 1);

    /* Allocate and compute Euler's totient up to max_q */
    uint32_t *phi = (uint32_t *)malloc((max_q + 1) * sizeof(uint32_t));
    if (!phi)
        return 0;  // Allocation failure, return zero

    for (size_t i = 0; i <= max_q; ++i)
        phi[i] = (uint32_t)i;

    for (size_t p = 2; p <= max_q; ++p)
    {
        if (phi[p] == (uint32_t)p)  /* p is prime */
        {
            for (size_t k = p; k <= max_q; k += p)
                phi[k] -= phi[k] / (uint32_t)p;
        }
    }

    seq_t result = 0;

    for (size_t q = 1; q <= max_q; ++q)
    {
        seq_t phi_q = (seq_t)phi[q];
        seq_t f = 0;

        /* Check if residue 0 has at least two occurrences */
        if (n - (idx_t)q >= (idx_t)q)
            f += 1;

        /* Number of residues r>0 with r ≤ n-q */
        size_t t = (size_t)(n - (idx_t)q);  /* always ≥1 because q ≤ n-1 */
        if (t >= 1)
        {
            size_t add = (t < (q - 1)) ? t : (q - 1);
            f += (seq_t)add;
        }

        result += phi_q * f;
    }

    free(phi);
    return result;
}
