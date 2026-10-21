/*
 * A253493  Number of (n+1) X (6+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 14306 + 1365*2^n + 49*3^(n-1)   for n > 3
 * Initial values: a(1) = 19701, a(2) = 20975, a(3) = 25715
 */
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Ensure the function works for all n >= 1.  For n == 0 we return 0. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)19701;
    if (n == 2) return (seq_t)20975;
    if (n == 3) return (seq_t)25715;

    /* Use the closed‑form formula. */
    seq_t res = (seq_t)14306;
    seq_t two_pow = pow2(n);           /* 2^n  */
    seq_t three_pow = pow3(n - 1);     /* 3^(n-1) */

    res += two_pow * (seq_t)1365;
    res += three_pow * (seq_t)49;

    return res;
}
