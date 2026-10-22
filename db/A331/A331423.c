/*
 * A331423  Divide each side of a triangle into n>=1 equal parts and trace the
 *          corresponding cevians, i.e., join every point, except for the
 *          first and last ones, with the opposite vertex. a(n) is the number
 *          of points at which three cevians meet.
 * https://oeis.org/A331423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of integer solutions of (n-i)*(n-j)*(n-k) = i*j*k
 *            with 0 < i, j, k < n
 * Range:     u8 n <= 71, u16 n <= 1045, u32 n <= 1045, u64 n <= 1045,
 *            u128 n <= 1045, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    const seq_t n_seq = (seq_t)n;

    for (idx_t i = 1; i < n; ++i)
    {
        const idx_t ni = n - i;
        const unsigned long long i_val = (unsigned long long)i;
        const unsigned long long ni_val = (unsigned long long)ni;

        for (idx_t j = 1; j < n; ++j)
        {
            const idx_t nj = n - j;
            const unsigned long long j_val = (unsigned long long)j;
            const unsigned long long nj_val = (unsigned long long)nj;

            const unsigned long long A = i_val * j_val;          /* i*j   */
            const unsigned long long B = ni_val * nj_val;        /* (n-i)(n-j) */

            const unsigned long long denom = A + B;              /* A+B   */
            if (denom == 0) continue;                            /* safety */

            const unsigned long long numer = B * (unsigned long long)n_seq;
            if (numer % denom != 0) continue;

            const unsigned long long k = numer / denom;
            if (k > 0 && k < n_seq) ++count;
        }
    }

    return count;
}
