/*
 * A276950  Characteristic function for A273670: 1 if there is at least one
 *          maximal digit present in the factorial representation of n
 *          (A007623), otherwise 0.
 * https://oeis.org/A276950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 941, u16 n <= 5040, u32 n <= 5040, u64 n <= 5040,
 *            u128 n <= 5040, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Characteristic function for A273670
   Return 1 if the factorial representation of n (base‑factorial
   system) contains at least one digit that equals its position
   (the maximal allowed digit).  Otherwise return 0. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t remaining = (seq_t)n;
    if (remaining == 0)
        return 0;

    /* 20! < 2^64 < 21!; this bound works for 8, 16, 32, 64‑bit
       unsigned types and for arbitrary‑precision seq_t. */
    seq_t fac[20];
    idx_t kmax = 0;
    seq_t cur = 1;

    for (idx_t k = 1; k <= 20; ++k) {
        unsigned __int128 prod = (unsigned __int128)cur * (unsigned __int128)k;
        if (prod > (unsigned __int128)remaining)
            break;
        fac[k - 1] = (seq_t)prod;
        cur = fac[k - 1];
        kmax = k;
    }

    for (idx_t k = kmax; k > 0; --k) {
        seq_t f   = fac[k - 1];
        seq_t d   = remaining / f;
        if (d == k)
            return 1;
        remaining -= d * f;
    }
    return 0;
}
