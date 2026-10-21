/*
 * A259239  E.g.f.: exp(x-sqrt(1-x^2)+1).
 * https://oeis.org/A259239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*n-3)*a(n-1) + (n-1)*(n^2-5*n+5)*a(n-2) * +
 *            2*(n-1)*(n-2)*(n-3)*(a(n-4)-a(n-3)))/(n-2) * Initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=4 * (Periodic memoized, thread‑safe
 *            for multiple calls in any order)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* total allocated entries */
    static size_t len   = 0;   /* highest computed index + 1 */

    /* Allocate or expand cache if needed */
    if ((idx_t)n >= (idx_t)cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        idx_t ii = (idx_t)i;
        if (ii == 0) {
            cache[i] = (seq_t)1;
        } else if (ii == 1) {
            cache[i] = (seq_t)1;
        } else if (ii == 2) {
            cache[i] = (seq_t)2;
        } else if (ii == 3) {
            cache[i] = (seq_t)4;
        } else {
            /* Branch depending on the width of seq_t:
             *   If seq_t is 64‑bit or smaller we use __int128 for intermediate
             *   to avoid unsigned wrap‑around.  For wider seq_t (or arbitrary‑
             *   precision) we perform the calculation directly with seq_t,
             *   taking care to avoid subtracting a larger number from a smaller
             *   one.  The sign of the third term is handled explicitly.     */
            if (sizeof(seq_t) <= 8) {
                /* Use signed __int128 intermediate */
                __int128 t1 = (__int128)(2 * ii - 3) *
                              (__int128)cache[i - 1];
                __int128 t2 = (__int128)(ii - 1) *
                              (__int128)(ii * ii - 5 * ii + 5) *
                              (__int128)cache[i - 2];
                __int128 coeff = (__int128)2 *
                                 (__int128)(ii - 1) *
                                 (__int128)(ii - 2) *
                                 (__int128)(ii - 3);
                __int128 diff;
                int sign;
                if (cache[i - 4] >= cache[i - 3]) {
                    diff = (__int128)cache[i - 4] -
                           (__int128)cache[i - 3];
                    sign = 1;
                } else {
                    diff = (__int128)cache[i - 3] -
                           (__int128)cache[i - 4];
                    sign = -1;
                }
                __int128 numerator = t1 + t2 + sign * coeff * diff;
                cache[i] = (seq_t)(numerator / (ii - 2));
            } else {
                /* Use seq_t arithmetic directly (assumes seq_t
                 * supports signed subtraction or comparison). */
                seq_t t1 = (seq_t)((2 * ii - 3) *
                                  cache[i - 1]);
                seq_t t2 = (seq_t)((ii - 1) *
                                  (ii * ii - 5 * ii + 5) *
                                  cache[i - 2]);
                seq_t coeff =
                    (seq_t)(2 * (ii - 1) * (ii - 2) * (ii - 3));
                seq_t diff;
                int sign;
                if (cache[i - 4] >= cache[i - 3]) {
                    diff = cache[i - 4] - cache[i - 3];
                    sign = 1;
                } else {
                    diff = cache[i - 3] - cache[i - 4];
                    sign = -1;
                }
                seq_t numerator = t1 + t2;
                if (sign > 0)
                    numerator += coeff * diff;
                else
                    numerator -= coeff * diff;
                cache[i] = numerator / (ii - 2);
            }
        }
    }
    len = (size_t)n + 1;
    return cache[n];
}
