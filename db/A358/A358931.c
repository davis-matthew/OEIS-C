/*
 * A358931  a(n) is the smallest n-gonal pyramidal number with binary weight
 *          n.
 * https://oeis.org/A358931
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 31 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the smallest n-gonal pyramidal number having
   binary weight n.  We compute it by brute‑force searching the layer
   index k.  The pyramidal number is
        P_m(k) = k(k+1)(2m(k-1) - 4k + 10)/12
   (m = n).  The binary weight is counted by repeated “x & (x-1)”. */

#include <stdint.h>

static seq_t pyramidal(idx_t m, idx_t k)
{
    /* use 128‑bit if available for the 64‑bit build */
#ifdef __SIZEOF_INT128__
    __int128 t1 = (__int128)k * (k + 1);
    __int128 t2 = (__int128)2 * m * (k - 1) - (__int128)4 * k + 10;
    __int128 prod = t1 * t2;
    prod /= 12;
    return (seq_t)prod;
#else
    seq_t t1 = (seq_t)k * (k + 1);
    seq_t t2 = (seq_t)2 * m * (k - 1) - (seq_t)4 * k + 10;
    seq_t prod = t1 * t2;
    prod /= 12;
    return prod;
#endif
}

static int bitcount(seq_t x)
{
    int c = 0;
    while (x)
    {
        x &= x - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;          /* undefined for offset < 3 */

    idx_t k = 1;
    for (;; ++k)
    {
        seq_t val = pyramidal(n, k);
        if (bitcount(val) == n)
            return val;
    }
}
