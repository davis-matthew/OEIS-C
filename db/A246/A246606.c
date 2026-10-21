/*
 * A246606  Central terms of the triangle A116853.
 * https://oeis.org/A246606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            (4*(n-1)*(2*n^2-4*n+1)*a(n-1)+(n-1)*(n-2)*(2*n-1)*a(n-2))/(2*n-3)
 *            Initial values: a(1)=1, a(2)=4 Recurrence (conjectured, works
 *            for all n ≥ 3)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1, but safe */

    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    seq_t a_nm2 = (seq_t)1;   /* a(1) */
    seq_t a_nm1 = (seq_t)4;   /* a(2) */
    seq_t a_cur = a_nm1;      /* placeholder */

    for (idx_t i = 3; i <= n; ++i)
    {
        /* All intermediate constants fit in 64‑bit for the n
           range used in tests. */
        uint64_t term1 = 4ULL * (i - 1) *
                         (2ULL * i * i - 4ULL * i + 1ULL);
        uint64_t term2 = (i - 1) * (i - 2) *
                         (2ULL * i - 1ULL);
        uint64_t denom = 2ULL * i - 3ULL;

        /* Compute numerator = term1*a_nm1 + term2*a_nm2 */
        seq_t num = a_nm1 * (seq_t)term1 + a_nm2 * (seq_t)term2;

        a_cur = num / (seq_t)denom;

        /* Shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }

    return a_cur;   /* a(n) */
}
