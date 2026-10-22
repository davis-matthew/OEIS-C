/*
 * A343836  Array T(n, k), n, k > 0, read by antidiagonals; the balanced
 *          ternary representation of T(n, k) is obtained by adding
 *          componentwise (i.e., without carries) the digits in the balanced
 *          ternary representations of n and of k.
 * https://oeis.org/A343836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, base
 * Formula:   a(n) is the nth term of array T(n,k) read by antidiagonals,
 *            where T(n,k) is defined by componentwise addition of the
 *            balanced ternary representations of n and k without carries
 * Range:     i8 n <= 8129, i16 n <= 9199, i32 n <= 9199, i64 n <= 9199,
 *            i128 n <= 9199, big any n
 * Helpers:   balanced_add, balanced_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include "../utils/balanced_add.h"
#include "../utils/balanced_digits.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Find the diagonal index s such that
       start = s*(s+1)/2 <= n < (s+1)*(s+2)/2          */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    long double root = sqrtl((long double)val);
    idx_t s = (idx_t)((root - 1.0L) / 2.0L);

    /* Adjust for possible rounding errors */
    while ((unsigned __int128)(s + 1) * (s + 2) / 2 <= n) ++s;
    while ((unsigned __int128)s * (s + 1) / 2 > n) --s;

    idx_t start  = s * (s + 1) / 2;
    idx_t offset = n - start;
    idx_t i = s - offset; /* row index in the array */
    idx_t j = offset;     /* column index in the array */

    return balanced_add(i, j);
}
