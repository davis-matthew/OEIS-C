/*
 * A322744  Array T(n,k) = (3*n*k - A319929(n,k))/2, n >= 1, k >= 1, read by
 *          antidiagonals.
 * https://oeis.org/A322744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 182, u16 n <= 861, u32 n <= 861, u64 n <= 861,
 *            u128 n <= 861, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A322744:  a(n) = T(i,j) read by antidiagonals,
   where T(i,j) = (3*i*j - A319929(i,j))/2
   and:
   A319929(i,j) = i + j - 1   if i and j are odd
                  i            if i even   and j odd
                  j            if i odd    and j even
                  0            if i and j are even.
   The indices i,j are 1‑based. */

#include <stddef.h>
#include <stdint.h>

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    /*--- find the diagonal (d) containing position n ---*/
    idx_t lo = 1;
    idx_t hi = n;              /* at most n rows/cols for n>=1 */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        /* triangular number mid*(mid+1)/2 should be >= n to be on this diagonal */
        __int128 mid128 = ( __int128 ) mid;
        __int128 tri = mid128 * (mid128 + 1) / 2;  /* 128‑bit to avoid overflow */
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t d = lo;   /* diagonal index (1‑based) */

    /* previous triangular number (before this diagonal) */
    __int128 d128 = ( __int128 ) d;
    __int128 prev_tri = (d128 - 1) * d128 / 2;
    idx_t prev_tri64 = (idx_t)prev_tri;          /* fits into idx_t */

    /* position inside the diagonal (0‑based) */
    idx_t pos = n - prev_tri64 - 1;

    /* i and j (both 1‑based) */
    idx_t i = pos + 1;
    idx_t j = d - pos;

    /*--- compute T(i,j) ---*/
    seq_t seq_i = (seq_t)i;
    seq_t seq_j = (seq_t)j;

    /* product 3*i*j in 128‑bit then cast */
    __int128 prod128 = ( __int128 )i * j * 3;
    seq_t prod = (seq_t)prod128;

    /* Decide which A319929 value to subtract based on parity */
    if ((i & 1) && (j & 1)) {                     /* both odd */
        seq_t sub = seq_i + seq_j - (seq_t)1;
        return (prod - sub) / (seq_t)2;
    } else if (!(i & 1) && (j & 1)) {             /* i even, j odd */
        return (prod - seq_i) / (seq_t)2;
    } else if ((i & 1) && !(j & 1)) {             /* i odd, j even */
        return (prod - seq_j) / (seq_t)2;
    } else {                                      /* both even */
        return prod / (seq_t)2;
    }
}
