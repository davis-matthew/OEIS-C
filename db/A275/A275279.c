/*
 * A275279  Least k such that d(k*n) = d(k*(n+1)), or 0 if no such k exists (d
 *          = A000005).
 * https://oeis.org/A275279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that d(k*n) = d(k*(n+1)), or 0 if no such k
 *            exists. Uses the helper function divisor_count_u64(uint64_t)
 *            from the utils library
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return 0;          /* as specified by the OEIS entry */

    for (idx_t k = 1; ; ++k) {
        /* Compute products in 128‑bit to detect overflow before casting to 64‑bit. */
        unsigned __int128 prod1 = (unsigned __int128)k * (unsigned __int128)n;
        if (prod1 > (unsigned __int128)UINT64_MAX) return 0;
        unsigned __int128 prod2 = prod1 + (unsigned __int128)k;  /* k*(n+1) = prod1 + k */

        uint64_t d1 = divisor_count_u64((uint64_t)prod1);
        uint64_t d2 = divisor_count_u64((uint64_t)prod2);

        if (d1 == d2) return (seq_t)k;
    }
}
