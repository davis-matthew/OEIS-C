/*
 * A396913  Table read by falling antidiagonals: T(n,k) is the number of
 *          k-colorings of the faces of a n-trapezohedron up to antiprismatic
 *          symmetry, n >= 3, k >= 0.
 * https://oeis.org/A396913
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n‑3, s‑n) where s is the smallest integer with *
 *            (s‑2)(s‑1)/2 > n‑3; T(n,k) = A321791(2*n,k) – number of *
 *            k‑colorings of a 2n‑bead bracelet (up to dihedral symmetry)
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/big_pow.h"

/* --- Euler's totient for 64‑bit integers -------------------------------- */
static seq_t phi_u64(uint64_t n)
{
    seq_t result = (seq_t)n;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / (seq_t)p;
        }
    }
    if (n > 1) result -= result / (seq_t)n;
    return result;
}

/* --- Number of k‑colorings of a bracelet with m beads (up to dihedral
 *     symmetry) ---------------------------------------------------------- */
static seq_t bracelet_count(idx_t m, seq_t k)
{
    seq_t sum_rot = 0;
    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            seq_t phi_d   = phi_u64(d);
            seq_t pow1    = big_pow(k, (seq_t)(m / d));
            seq_t prod1   = (seq_t)((unsigned __int128)phi_d * pow1);
            sum_rot += prod1;

            idx_t d2 = m / d;
            if (d2 != d) {
                seq_t phi_d2 = phi_u64(d2);
                seq_t pow2   = big_pow(k, (seq_t)(m / d2));
                seq_t prod2   = (seq_t)((unsigned __int128)phi_d2 * pow2);
                sum_rot += prod2;
            }
        }
    }

    seq_t refl = 0;
    if (m & 1) {                           /* m is odd */
        seq_t pow = big_pow(k, (seq_t)((m + 1) / 2));
        refl = (seq_t)((unsigned __int128)m * pow);
    } else {                                /* m is even */
        seq_t term1 = big_pow(k, (seq_t)(m / 2 + 1));
        seq_t term2 = big_pow(k, (seq_t)(m / 2));
        seq_t sum_terms = term1 + term2;
        refl = (seq_t)((unsigned __int128)(m / 2) * sum_terms);
    }

    seq_t numer = sum_rot + refl;
    seq_t denom = (seq_t)2 * (seq_t)m;
    return numer / denom;
}

/* --- Main sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;           /* undefined for n < 3 */

    idx_t idx0 = n - 3;                   /* zero‑based index in the sequence */

    /* Find t such that t*(t+1)/2 > idx0, where t = s-2 and s = n+k. */
    long double D = 8.0L * (long double)idx0 + 1.0L;
    long double root_ld = sqrt(D);
    idx_t sqrtD = (idx_t)root_ld;          /* floor(sqrt(D)) */
    idx_t t = (sqrtD + 1) / 2;             /* initial guess */

    /* Correct possible rounding errors */
    while ((unsigned __int128)t * (t + 1) / 2 <= idx0) ++t;
    while (t > 0 && (unsigned __int128)(t - 1) * t / 2 > idx0) --t;

    idx_t s = t + 2;                      /* n + k */

    idx_t prev = (t - 1) * t / 2;          /* number of terms before sum s */
    idx_t offset = idx0 - prev;            /* position within this antidiagonal */

    idx_t row_n = 3 + offset;              /* n coordinate of the table entry */
    idx_t k = s - row_n;                   /* k coordinate of the table entry */

    /* Compute T(row_n, k) = A321791(2*row_n, k) */
    idx_t m = 2 * row_n;                   /* number of beads in the bracelet */
    seq_t seq_k = (seq_t)k;                /* color count */

    return bracelet_count(m, seq_k);
}
