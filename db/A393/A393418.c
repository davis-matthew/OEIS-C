/*
 * A393418  Expansion of (1/x) * Series_Reversion( x * (2 * (1 - x)^2 - 1) ).
 * https://oeis.org/A393418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A393418
   The recurrence is, for n ≥ 2,
        2*n*(n+1)*a(n) = 14*n*(2*n-1)*a(n-1)
                           + 3*(3*n-2)*(3*n-4)*a(n-2)
   Initial values:
        a(0) = 1
        a(1) = 4
*/

#include <stdint.h>

extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)4;   /* a(1) */
    seq_t ai = a1;

    for (idx_t i = 2; i <= n; ++i) {
        unsigned long long ii = (unsigned long long)i;
        unsigned long long c1 = 14ULL * ii * (2ULL * ii - 1ULL);
        unsigned long long c2 = 3ULL * (3ULL * ii - 2ULL) * (3ULL * ii - 4ULL);
        unsigned long long den = 2ULL * ii * (ii + 1ULL);

#ifdef __SIZEOF_INT128__
        /* Use 128‑bit intermediate to avoid overflow on 64‑bit seq_t */
        __int128 num = (__int128)c1 * (__int128)a1 + (__int128)c2 * (__int128)a0;
        ai = (seq_t)(num / den);
#else
        /* Fallback: rely on seq_t arithmetic (works for arbitrary‑precision types) */
        seq_t num = (seq_t)c1 * a1 + (seq_t)c2 * a0;
        ai = num / (seq_t)den;
#endif
        a0 = a1;
        a1 = ai;
    }

    return a1;
}
