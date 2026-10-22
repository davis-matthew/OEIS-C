/*
 * A318107  Triangle read by rows: T(n,k) = (3*n - 2*k)!/((n-k)!^3*k!).
 * https://oeis.org/A318107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the flattened triangle T(n,k) =
 *            (3n-2k)! / ((n-k)!^3 k!). Convert n to row r and column c with
 *            r*(r+1)/2 ≤ n < (r+1)(r+2)/2 then c = n - r*(r+1)/2. Finally
 *            a(n) = binom(3r-2c, r-c) * binom(2r-c, r-c) * binom(r, c)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 35, u64 n <= 119, u128 n <= 406,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helper function binom(idx_t a, idx_t b) returns the binomial coefficient
   C(a,b) as a seq_t.  The routines in utils are available in the build. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

/* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
   n is 0‑based. */
static idx_t find_row(idx_t n) {
    idx_t low = 0;
    idx_t high = 1;
    /* Grow high until it exceeds n. */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
    }
    /* Binary search between low and high. */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

seq_t a(idx_t n) {
    idx_t r = find_row(n);
    idx_t c = n - r * (r + 1) / 2;  /* column within the row */

    idx_t a_val = 3 * r - 2 * c;     /* 3r-2c  */
    idx_t b     = r - c;             /* r-c     */

    seq_t result = binom(a_val, b);          /* C(3r-2c, r-c) */
    result *= binom(2 * r - c, b);           /* C(2r-c,   r-c) */
    result *= binom(r, c);                   /* C(r,     c)   */

    return result;
}
