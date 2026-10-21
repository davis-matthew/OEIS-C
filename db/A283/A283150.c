/*
 * A283150  Riordan array (1/(1-9x)^(1/3), x/(9x-1)).
 * https://oeis.org/A283150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) enumerates the triangle a(m,n) defined by * a(m,n) =
 *            binomial(-n-1/3, m-n) * (-1)^m * 9^(m-n) * where m =
 *            floor((sqrt(8*n+1)-1)/2) and n' = n - m*(m+1)/2. * The
 *            implementation uses integer recurrences: * Base: a(m,0) = 1 *
 *            product_{i=0}^{m-1} (9*i+3)/(i+1) * Recurrence: a(m,k) =
 *            a(m,k-1) * (...
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute a(n) for the OEIS sequence A283150 */
seq_t a(idx_t n)
{
    /* Find row m such that m*(m+1)/2 <= n < (m+1)*(m+2)/2 */
    uint64_t low = 0;
    uint64_t high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;

    while (low < high) {
        uint64_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    uint64_t m = low;
    uint64_t row_start = (unsigned __int128)m * (m + 1) / 2;
    uint64_t r = n - row_start;                /* column index within row */

    /* Compute a(m,0) */
    seq_t val = 1;
    for (uint64_t i = 0; i < m; ++i) {
        __int128 num = (__int128)val * (9 * i + 3);
        val = (seq_t)(num / (i + 1));
    }

    /* Compute a(m,r) via the recurrence */
    for (uint64_t j = 1; j <= r; ++j) {
        __int128 num = (__int128)val *
                        ((long long)j - 1 - (long long)m);
        val = (seq_t)(num / (9 * j - 6));
    }

    return val;
}
