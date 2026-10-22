/*
 * A316858  Triangle read by rows constructed from A090368 as sum of least
 *          prime factors.
 * https://oeis.org/A316858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = lpf(2*i+1) + lpf(2*(k+1-i)+1) where k is the row number
 *            with T(k-1) < n ≤ T(k) (T(k)=k(k+1)/2) and i = n – T(k-1). This
 *            implements the triangular array described in OEIS A316858
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   _row_for_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_row_for_n.h"

/* least prime factor for small odd numbers (n ≥ 2) */
static seq_t _lpf_uint64(uint64_t n)
{
    if (n <= 1) return (seq_t)n;
    for (uint64_t p = 2; (__int128)p * p <= n; ++p)
        if (n % p == 0) return (seq_t)p;
    return (seq_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1 – return 0 */

    idx_t k = _row_for_n(n);
    /* previous triangle size T(k-1) = k(k-1)/2 */
    __int128 prev_tri = (__int128)k * (k - 1) / 2;
    idx_t i = n - (idx_t)prev_tri;   /* position in the row, 1..k */

    uint64_t num1 = 2 * (uint64_t)i + 1;
    uint64_t num2 = 2 * (uint64_t)(k + 1 - i) + 1;

    seq_t lpf1 = _lpf_uint64(num1);
    seq_t lpf2 = _lpf_uint64(num2);

    return lpf1 + lpf2;
}
