/*
 * A369269  Expansion of (1/x) * Series_Reversion( x * (1-x)^2 / (1+x^3)^3 ).
 * https://oeis.org/A369269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/3⌋} binomial(3*n+3, k) *
 *            binomial(3*n-3*k+1, n-3*k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Number of terms to sum */
    idx_t kmax = n / 3;

    /* Pre‑compute the common parameters */
    idx_t N = 3 * n + 3;              /* first binomial top */
    idx_t M = 3 * n + 1;              /* second binomial top */
    idx_t L = n;                      /* second binomial bottom */

    /* b1 = binomial(N,0) = 1, b2 = binomial(M,L) */
    seq_t b1 = 1;
    seq_t b2 = binomial(M, L);       /* use the utility */

    seq_t sum = 0;

    for (idx_t k = 0; k <= kmax; ++k) {
        /* accumulate the term */
        sum += b1 * b2;

        if (k == kmax)
            break;       /* last term processed */

        /* ---------- update b1 : binomial(N,k+1) ---------- */
        b1 = b1 * (seq_t)(N - k) / (seq_t)(k + 1);

        /* ---------- update b2 : binomial(M-3, L-3) ---------- */
        idx_t Mtmp = M;
        idx_t Ltmp = L;

        /* Step 1 : multiply by L, divide by M */
        seq_t tmp = mul_small(b2, (unsigned int)Ltmp);
        b2 = tmp / (seq_t)Mtmp;
        Mtmp--;  Ltmp--;

        /* Step 2 : multiply by (L-1), divide by (M-1) */
        tmp = mul_small(b2, (unsigned int)Ltmp);
        b2 = tmp / (seq_t)Mtmp;
        Mtmp--;  Ltmp--;

        /* Step 3 : multiply by (L-2), divide by (M-2) */
        tmp = mul_small(b2, (unsigned int)Ltmp);
        b2 = tmp / (seq_t)Mtmp;

        /* advance to next values of M and L */
        M -= 3;
        L -= 3;
    }

    /* final division by (n+1) */
    return sum / (seq_t)(n + 1);
}
