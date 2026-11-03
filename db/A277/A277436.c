/*
 * A277436  a(n) = |Gamma(n, i)|^2, where i = sqrt(-1).
 * https://oeis.org/A277436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |Gamma(n, i)|^2 Recurrence: a(n+1) = n^2*a(n) + 1 -
 *            n*(n-1)*(a(n-1) - (n-2)^2*a(n-2) + 1) Initial values: a(1) = 1,
 *            a(2) = 2, a(3) = 5
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

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

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)5;

    seq_t a_nm3 = (seq_t)1; /* a(1) */
    seq_t a_nm2 = (seq_t)2; /* a(2) */
    seq_t a_nm1 = (seq_t)5; /* a(3) */
    seq_t result = 0;

    for (idx_t m = 4; m <= n; ++m) {
        unsigned int nn = (unsigned int)(m - 1);          /* n in recurrence */
        seq_t term1 = mul_small(a_nm1, nn * nn) + (seq_t)1;

        seq_t temp  = mul_small(a_nm3, (unsigned int)((nn - 2) * (nn - 2)));
        seq_t inner = a_nm2 - temp + (seq_t)1;

        seq_t term2 = mul_small(inner, nn * (nn - 1));

        result = term1 - term2;

        /* shift for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = result;
    }
    return result;
}
