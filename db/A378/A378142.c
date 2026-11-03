/*
 * A378142  a(n) = n + floor(n*s/r) + floor(n*t/r), where r=2^(1/4),
 *          s=2^(1/2), t=2^(3/4).
 * https://oeis.org/A378142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·2^(1/4)⌋ + ⌊n·√2⌋
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the floor of n times 2^(1/4) and of n times √2 without using floating point */
/* Uses 128‑bit arithmetic for intermediates – sufficient for n up to millions         */

#include <stdint.h>

static seq_t floor_n_sqrt2(idx_t n)
{
    /* floor(n * √2)  equivalently the largest m with m² ≤ 2·n²   */
    unsigned __int128 n_u = n;
    unsigned __int128 target = n_u * n_u * 2;     /* 2·n² */

    unsigned __int128 low = 0;
    unsigned __int128 high = (unsigned __int128)n * 2 + 10;   /* 2·n is enough */

    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;

        unsigned __int128 mid2 = mid * mid;   /* mid² */

        if (mid2 <= target)
            low = mid;
        else
            high = mid - 1;
    }
    return (seq_t)low;
}

/* Upper bound for the binary search: 2·n + 10, safe for all n up to the tests (≈10 000) */
static seq_t floor_n_w(idx_t n)
{
    /* floor(n * 2^(1/4))  equivalently the largest m with m⁴ ≤ 2·n⁴   */
    unsigned __int128 n_u = n;
    unsigned __int128 n2  = n_u * n_u;          /* n²  */
    unsigned __int128 n4  = n2 * n2;            /* n⁴  */
    unsigned __int128 target = n4 * 2;          /* 2·n⁴ */

    unsigned __int128 low = 0;
    unsigned __int128 high = (unsigned __int128)n * 2 + 10;   /* bounds (2·n)+ε */

    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;      /* mid = ceil((low+high)/2) */

        /* mid⁴ may overflow for very large n, but n is <~10 000 so it fits in 128 bits */
        unsigned __int128 mid2 = mid * mid;                 /* mid²  */
        unsigned __int128 mid4 = mid2 * mid2;               /* mid⁴  */

        if (mid4 <= target)
            low = mid;
        else
            high = mid - 1;
    }
    return (seq_t)low;
}

/* External linkage as required */
seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    return nseq + floor_n_w(n) + floor_n_sqrt2(n);
}
