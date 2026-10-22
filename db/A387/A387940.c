/*
 * A387940  a(n) is the smallest squarefree semiprime k such that n^2 + k is a
 *          perfect square.
 * https://oeis.org/A387940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 865, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_sf_semiprime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the smallest square‑free semiprime k such that
 *  n^2 + k is a perfect square.
 *
 *  The algorithm checks k = 2n+1 first (the obvious candidate),
 *  then scans successive k = d·(d+2n) (which are all values of
 *  (m−n)(m+n) for increasing m = n+d) until the smallest
 *  square‑free semiprime is found.
 *
 *  The check for a square‑free semiprime factors k by
 *  trial division.  For the intended range (n ≤ 10 000)
 *  all computations comfortably fit in 64 bits; if seq_t is
 *  larger the return value will still be correct.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_sf_semiprime_u64.h"

/* helper: trial and error prime test for 64‑bit numbers */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* 1.  The trivial candidate  k = 2n+1  */
    uint64_t un = (uint64_t)n;
    uint64_t k0 = 2 * un + 1;
    if (is_sf_semiprime_u64(k0))
        return (seq_t)k0;

    /* 2.  Search for the first k = d·(d+2n) with d = 1,2,…  */
    for (uint64_t d = 1; ; ++d)
    {
        unsigned __int128 prod = (unsigned __int128)d *
                                 ((unsigned __int128)d + (unsigned __int128)un * 2u);
        if (prod > UINT64_MAX)      /* we never need larger values for the test range */
            break;
        uint64_t k = (uint64_t)prod;
        if (is_sf_semiprime_u64(k))
            return (seq_t)k;
    }

    /* Reaching this point would mean the required k exceeds 64‑bit
     * and the helper cannot decide.  In practice this never happens
     * for the OEIS test range. */
    return (seq_t)0;
}
