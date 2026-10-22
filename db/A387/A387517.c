/*
 * A387517  For n >= 0, a(n) is the least s >= 0 such that (s + 1)*(s + n) is
 *          a triangular number (A000217).
 * https://oeis.org/A387517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 259, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A387517 – least s ≥ 0 such that (s+1)*(s+n) is a triangular number.
   The function performs a simple brute‑force search over s in the range
   0 … n-3 (the OEIS comment guarantees a solution in this range for
   n ≥ 3).  To test whether (s+1)*(s+n) is triangular it checks whether
   8·(s+1)·(s+n)+1 is a perfect square.  The check is done on an
   unsigned __int128 value, which is large enough for all n up to the
   limits required by the tests. */

#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Helper: test whether a unsigned __int128 value is a perfect square.
    Uses binary search.  The function is independent of seq_t.          */
/* ------------------------------------------------------------------ */
static bool is_square_uint128(unsigned __int128 val)
{
    unsigned __int128 low  = 0;
    unsigned __int128 high = val;

    while (low <= high) {
        unsigned __int128 mid = low + ((high - low) >> 1);
        unsigned __int128 sq  = mid * mid;

        if (sq == val)
            return true;
        if (sq < val)
            low = mid + 1;
        else {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    return false;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function.                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Handle the small indices explicitly (no underflow of n-3). */
    if (n == 0 || n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)1;

    /* For n ≥ 3 search for the smallest s with the property. */
    for (idx_t s = 0; s <= n - 3; ++s) {
        unsigned __int128 prod = (unsigned __int128)(s + 1) *
                                 (unsigned __int128)(s + n);
        prod = prod * (unsigned __int128)8 + (unsigned __int128)1;

        if (is_square_uint128(prod))
            return (seq_t)s;
    }

    /* According to the OEIS comment a solution always exists for n ≥ 3.
       The following return is only to keep the function total. */
    return (seq_t)(n - 3);
}
