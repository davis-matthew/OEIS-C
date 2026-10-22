/*
 * A377279  Number of fixed points of f(k) = floor(k^2 / n) mod n^2.
 * https://oeis.org/A377279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k ∈ [0, n²-1] : floor(k² / n) ≡ k (mod n²) }| (number
 *            of fixed points of f(k) = floor(k^2 / n) mod n^2)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* safety for out‑of‑range calls */

    idx_t N = n;
    unsigned __int128 N128   = N;
    unsigned __int128 Nsq128 = N128 * N128;
    signed __int128   Nsq128s= (signed __int128)Nsq128;

    /* Pre‑compute c[r] = floor(r^2 / n) for r = 0 … N-1 */
    uint32_t *c = (uint32_t *)malloc(N * sizeof(uint32_t));
    if (!c) return 0;                  /* allocation failed */
    for (idx_t r = 0; r < N; ++r)
        c[r] = (uint32_t)((r * r) / N);

    seq_t count = 0;

    for (idx_t q = 0; q < N; ++q) {
        /* term1 = n*q*(q-1)  (may be ≥ 0) */
        signed __int128 term1    = (signed __int128)N128 * q * (q - 1);
        signed __int128 term1_mod = term1 % Nsq128s;

        /* coefficient of r : (2*q - 1) */
        signed __int128 coef    = (signed __int128)(2 * q - 1);
        signed __int128 coef_mod = coef % Nsq128s;   /* may be negative */

        /* initial value for r = 0 */
        signed __int128 value_mod = term1_mod + c[0];
        if (value_mod >= Nsq128s) value_mod -= Nsq128s;
        if (value_mod < 0)        value_mod += Nsq128s;

        if (value_mod == 0) ++count;

        for (idx_t r = 1; r < N; ++r) {
            /* value_mod += coef_mod + (c[r] - c[r-1])  (mod n²) */
            value_mod += coef_mod;
            if (value_mod >= Nsq128s) value_mod -= Nsq128s;
            if (value_mod < 0)        value_mod += Nsq128s;

            value_mod += (c[r] - c[r - 1]);      /* diff ≥ 0, small (≤3) */
            if (value_mod >= Nsq128s) value_mod -= Nsq128s;
            if (value_mod < 0)        value_mod += Nsq128s;

            if (value_mod == 0) ++count;
        }
    }

    free(c);
    return count;
}
