/*
 * A389773  One third the number of solid partitions of n with 6 parts.
 * https://oeis.org/A389773
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the coefficient of q^n in (4*q^17 + 5*q^16 + 5*q^15 +
 *            22*q^14 + 27*q^13 + 25*q^12 + 24*q^11 + 32*q^10 + 30*q^9 +
 *            21*q^8 + 16*q^6) / prod_{k=1..6}(1 - q^k) The computation is
 *            performed by dividing the polynomial P by the polynomial Q =
 *            prod_{k=1..6}(1 - q^k) using successive division by (1...
 * Range:     u8 n <= 10, u16 n <= 31, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Numerator coefficients */
static int Pcoeff[22] = {0};

static void init_Pcoeff(void)
{
    /* Only set the non‑zero coefficients */
    Pcoeff[6]  = 16;
    Pcoeff[8]  = 21;
    Pcoeff[9]  = 30;
    Pcoeff[10] = 32;
    Pcoeff[11] = 24;
    Pcoeff[12] = 25;
    Pcoeff[13] = 27;
    Pcoeff[14] = 22;
    Pcoeff[15] = 5;
    Pcoeff[16] = 5;
    Pcoeff[17] = 4;
}

seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        init_Pcoeff();
        initialized = 1;
    }

    /* Allocate series up to n */
    seq_t *S = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!S) return 0;                   /* Out of memory: return 0 */

    /* Zero initialise */
    for (idx_t i = 0; i <= n; ++i) S[i] = 0;

    /* Load numerator coefficients */
    for (idx_t i = 0; i <= n && i < 22; ++i)
        if (Pcoeff[i] != 0)
            S[i] = (seq_t)Pcoeff[i];

    /* Divide by each factor (1 - q^k) for k = 1..6 */
    for (int k = 1; k <= 6; ++k) {
        for (idx_t i = k; i <= n; ++i) {
            S[i] += S[i - k];
        }
    }

    seq_t result = S[n];
    free(S);
    return result;
}
