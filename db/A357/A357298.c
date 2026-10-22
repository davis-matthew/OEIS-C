/*
 * A357298  Triangle read by rows where all entries in every even row are 1's
 *          and the entries in every odd row alternate between 0 (start/end)
 *          and 1.
 * https://oeis.org/A357298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 136, u16 n <= 136, u32 n <= 136, u64 n <= 136,
 *            u128 n <= 136, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n‑th element of a triangle where
 * all even rows are all 1’s and all odd rows alternate 0/1
 * starting with 0.  Thus a(1)=0, a(2)=1, a(3)=1, a(4)=0, … 
 */
#include <stdint.h>   /* for uintptr_t, if needed */

seq_t a(idx_t n)
{
    /* any n below the offset (n<1) is irrelevant, return 0 */
    if (n == 0) return (seq_t)0;

    /* Binary search for the row r such that T(r-1) < n <= T(r),
     * where T(r) = r*(r+1)/2.  Use 128‑bit arithmetic to avoid overflows. */
    idx_t low  = 1;
    idx_t high = n;   /* r cannot exceed n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1u) / 2u;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;   /* row number (1‑based) */

    /* Position within the row: 0‑based index k */
    unsigned __int128 t_prev128 = (unsigned __int128)(r - 1) * r / 2u;
    idx_t t_prev = (idx_t)t_prev128;
    idx_t k = n - t_prev - 1;   /* 0 <= k < r */

    /* Even rows are all 1’s.  Odd rows alternate 0/1 starting with 0. */
    if ((r & 1u) == 0u)        /* even row */
        return (seq_t)1;
    else                      /* odd row */
        return (seq_t)((k & 1u) ? 1 : 0);
}
