/*
 * A331359  Number of unoriented colorings of the edges of a tesseract with n
 *          available colors.
 * https://oeis.org/A331359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (48*n^4 + 64*n^6 + 164*n^8 + 32*n^12 + 35*n^16 + 24*n^18
 *            + 16*n^20 + n^32) / 384
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 12,
 *            big any n
 * Helpers:   compute_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/compute_big.h"

static seq_t compute_small(idx_t n)
{
    unsigned __int128 N = n;        // 64‑bit unsigned cast
    unsigned __int128 n2 = N * N;          // n^2
    unsigned __int128 n4 = n2 * n2;        // n^4
    unsigned __int128 n6 = n4 * n2;        // n^6
    unsigned __int128 n8 = n4 * n4;        // n^8
    unsigned __int128 n12 = n8 * n4;       // n^12
    unsigned __int128 n16 = n8 * n8;       // n^16
    unsigned __int128 n18 = n16 * n2;      // n^18
    unsigned __int128 n20 = n16 * n4;      // n^20
    unsigned __int128 n32 = n16 * n16;     // n^32

    unsigned __int128 numerator =
          (unsigned __int128)48 * n4
        + (unsigned __int128)64 * n6
        + (unsigned __int128)164 * n8
        + (unsigned __int128)32 * n12
        + (unsigned __int128)35 * n16
        + (unsigned __int128)24 * n18
        + (unsigned __int128)16 * n20
        + n32;

    return (seq_t)(numerator / 384);
}

seq_t a(idx_t n)
{
    /* For small integer types use 128‑bit intermediate to avoid overflow
       on the intermediate powers. For arbitrary‑precision types use
       the big computation directly. */
    if (sizeof(seq_t) <= sizeof(uint64_t))
        return compute_small(n);
    else
        return compute_big(n);
}
