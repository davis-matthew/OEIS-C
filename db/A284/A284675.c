/*
 * A284675  Positions of 0 in A284674; complement of A284676.
 * https://oeis.org/A284675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊(5n + ⌊n√13⌋)/2⌋ – d(n), where d(n) = 3 if (5n+⌊n√13⌋)
 *            is even and ⌊n√13⌋ ≥ ⌊(n–1)√13⌋+4, otherwise d(n) = 2. For n = 1
 *            we set d(1) = 3
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root of a 128‑bit unsigned integer.
   Returns floor(sqrt(x)). */
static inline uint64_t
isqrt128 (unsigned __int128 x)
{
    uint64_t lo = 0;
    uint64_t hi = (uint64_t) -1;        /* 2^64-1, which is ≥ sqrt(x) for any 128‑bit x */
    while (lo <= hi)
    {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a (idx_t n)
{
    /* Special case for n < 1 is ignored because the offset is 1. */
    if (n == 0)
        return (seq_t)0;

    uint64_t N = (uint64_t)n;                 /* 64‑bit copy of the index */

    /* S_n  = ⌊n√13⌋  computed as ⌊√(13·n²)⌋ */
    unsigned __int128 prod_n  = (unsigned __int128)13 * N * N;
    uint64_t        S_n      = (uint64_t)isqrt128(prod_n);

    /* S_{n-1} for the parity test */
    uint64_t S_prev;
    if (N == 1)
        S_prev = 0;
    else
    {
        unsigned __int128 prod_prev = (unsigned __int128)13 * (N - 1) * (N - 1);
        S_prev = (uint64_t)isqrt128(prod_prev);
    }

    /* T = 5·n + ⌊n√13⌋  (used for the floor division and parity) */
    unsigned __int128 T = (unsigned __int128)5 * N + S_n;

    /* Decide the subtraction constant d(n) */
    int d;
    if (N == 1)
        d = 3;
    else
    {
        bool T_even = ((T & 1) == 0);            /* T is even ? */
        if (T_even && (S_n >= S_prev + 4ULL))
            d = 3;
        else
            d = 2;
    }

    /* a(n) = ⌊T/2⌋ – d */
    unsigned __int128 a_floor = T >> 1;          /* division by 2 */
    unsigned __int128 a_val   = a_floor - d;

    return (seq_t)a_val;                          /* cast back to seq_t */
}
