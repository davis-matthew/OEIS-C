/*
 * A309990  Digits of one of the two 17-adic integers sqrt(-1).
 * https://oeis.org/A309990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit n (0‑based) of the 17‑adic integer √‑1 with the
 *            first digit 13. The function computes the digits by Hensel
 *            lifting of the square‑root equation x²≡‑1 (mod 17^k) and uses
 *            the standard convolution formula for the coefficient of 17^k in
 *            x². No large integers are required for the 64‑bit b...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The first digit is fixed to 13. */
    if (n == 0)
        return (seq_t)13;

    /* Use a small array of uint8_t to store the already computed digits. */
    uint8_t *dig = (uint8_t*)malloc((size_t)(n + 1) * sizeof(uint8_t));
    if (!dig)            /* allocation failure – terminate */
        return (seq_t)0; /* should never happen */
    dig[0] = 13;

    /* carry = (a0² + 1) / 17  → 10 for the first step. */
    int64_t carry = (13LL * 13LL + 1LL) / 17LL;

    /* Inverse of 2*a0 ≡ 26 ≡ 9 (mod 17); 9⁻¹ ≡ 2 (mod 17). */
    const int64_t inv9 = 2LL;       /* 2*a0≡9, inv9≡2 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* B = Σ_{i=1}^{k-1} a_i * a_{k-i}   (convolution coefficient). */
        int64_t B = 0;
        for (idx_t i = 1; i < k; ++i)
            B += (int64_t)dig[i] * (int64_t)dig[k - i];

        /* Solve 2*a0*a_k + B + carry ≡ 0 (mod 17)  →  a_k. */
        int64_t tmp = -(B + carry);                /* -(B+carry) */
        tmp %= 17LL;                               /* modulo 17 */
        if (tmp < 0) tmp += 17LL;                  /* make positive */
        int64_t d = (tmp * inv9) % 17LL;           /* a_k (0 … 16) */

        dig[k] = (uint8_t)d;

        /* Update carry for the next step:
           carry = (B + carry + 2*a0*d) / 17.        */
        int64_t T = B + carry + 2LL * 13LL * d;
        carry = T / 17LL;                          /* exact division */
    }

    seq_t result = (seq_t)dig[n];
    free(dig);
    return result;
}
