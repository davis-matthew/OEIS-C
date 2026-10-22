/*
 * A378010  a(n) = b(8*n+1), with the sequence {b(n)} having Dirichlet g.f.
 *          Product_{chi} L(chi,s), where chi runs through all Dirichlet
 *          characters modulo 8; 8th column of A378007.
 * https://oeis.org/A378010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(8*n+1) where b(n) is multiplicative with * b(2^e) = 0;
 *            * b(p^e) = binomial(e+3,3) if p ≡ 1 (mod 8); * b(p^e) = e/2 + 1
 *            if p ≡ 3,5,7 (mod 8) and e is even, * and 0 otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* m = 8*n + 1  (always odd) */
    uint64_t m = (uint64_t)n * 8ULL + 1ULL;
    if (m == 1ULL) return (seq_t)1;   /* b(1) = 1 */

    seq_t res = (seq_t)1;
    uint64_t temp = m;

    /* Since m is odd, the factor 2 cannot occur. */
    /* Trial‑division factorisation */
    for (uint64_t d = 3ULL; d * d <= temp; d += 2ULL) {
        if (temp % d == 0ULL) {
            uint64_t e = 0ULL;
            while (temp % d == 0ULL) {
                ++e;
                temp /= d;
            }

            seq_t contrib;
            if (d % 8ULL == 1ULL) {                    /* p ≡ 1 (mod 8) */
                seq_t ee = (seq_t)e;
                seq_t a_ = ee + 3;
                seq_t b_ = ee + 2;
                seq_t c_ = ee + 1;
                seq_t prod = a_ * b_;
                prod = prod * c_;
                prod = prod / 6;
                contrib = prod;                       /* binomial(e+3,3) */
            } else {                                   /* p ≡ 3,5,7 (mod 8) */
                if (e & 1ULL)                           /* odd exponent → 0 */
                    return (seq_t)0;
                contrib = (seq_t)(e / 2ULL + 1ULL);     /* e/2 + 1 */
            }

            res *= contrib;
            if (res == 0) return (seq_t)0;
        }
    }

    /* Remaining prime factor > sqrt(temp) (if any) */
    if (temp > 1ULL) {
        uint64_t p = temp;           /* exponent e = 1 */
        if (p % 8ULL == 1ULL) {
            res *= (seq_t)4;         /* binomial(4,3) = 4 */
        } else {                     /* p ≡ 3,5,7 (mod 8) and e=1 (odd) */
            return (seq_t)0;
        }
    }

    return res;
}
