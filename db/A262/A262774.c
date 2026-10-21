/*
 * A262774  Expansion of psi(x^2) * phi(-x^3) in powers of x where phi(),
 *          psi() are Ramanujan theta functions.
 * https://oeis.org/A262774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   The generating function of this sequence is ψ(x²) · φ(–x³) =
 *            ∑_{m≥0} x^{m(m+1)} · ( 1 + 2∑_{j≥1} (–1)^{j} x^{3j²} ) Therefore
 *            a(n) = ∑_{j≥0} c_j where c_0 = 1 if n−3·0² is a
 *            “triangular‑double” m(m+1), otherwise 0; for j≥1, c_j = 2·(–1)^j
 *            when n−3j² = m(m+1) for some integer m, else 0
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned 64‑bit values, floor(sqrt(x)) */
static unsigned long long isqrt_u64(unsigned long long x)
{
    unsigned long long lo = 0, hi = 1;
    while ((unsigned long long)hi * hi <= x) hi <<= 1;
    while (lo + 1 < hi) {
        unsigned long long mid = (lo + hi) >> 1;
        if ((unsigned long long)mid * mid <= x) lo = mid;
        else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* iterate over j such that 3*j*j ≤ n */
    for (idx_t j = 0;
         (unsigned long long)3 * ((unsigned long long)j * (unsigned long long)j) <= (unsigned long long)n;
         ++j)
    {
        unsigned long long rem = (unsigned long long)n
                                - (unsigned long long)3 * ((unsigned long long)j * (unsigned long long)j);

        /* solve m(m+1) = rem  <=>  m² + m – rem = 0  */
        unsigned long long D = 1 + 4 * rem;
        unsigned long long s = isqrt_u64(D);
        if (s * s != D) continue;                /* no integer solution for m */

        /* m = (s – 1)/2 automatically integer because D ≡ 1 (mod 4) */
        (void)((s - 1) / 2); /* m is not used further, only existence matters */

        if (j == 0) {
            result += 1;
        } else {
            /* weight 2·(–1)^j */
            result += (j & 1) ? -2 : 2;
        }
    }
    return result;
}
