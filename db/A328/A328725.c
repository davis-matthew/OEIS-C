/*
 * A328725  Constant term in the expansion of (1 + x + y + z + 1/x + 1/y + 1/z
 *          + x*y + y*z + z*x + 1/(x*y) + 1/(y*z) + 1/(z*x) + x*y*z +
 *          1/(x*y*z))^n.
 * https://oeis.org/A328725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n} (-1)^(n-i)*binomial(n,i)*Sum_{j=0..i}
 *            binomial(i,j)^4 * Recurrence: * n^3*a(n) = (2*n - 1)^3*a(n-1) *
 *            + (n-1)*(94*n^2 - 188*n + 93)*a(n-2) * + 80*(n-2)*(n-1)*(2*n -
 *            3)*a(n-3) * + 75*(n-3)*(n-2)*(n-1)*a(n-4) * Initial values: *
 *            a(0)=1, a(1)=1, a(2)=15, a(3)=115, a(4)=1255
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   compute_term_big, compute_term_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/compute_term_big.h"
#include "../utils/compute_term_small.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0;   /* number of entries already stored */

    if (n >= cap) {
        idx_t newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failed */
        for (idx_t i = cap; i <= n; ++i) {
            seq_t val;
            if (i <= 4) {
                switch ((int)i) {
                    case 0: case 1: val = (seq_t)1; break;
                    case 2: val = (seq_t)15; break;
                    case 3: val = (seq_t)115; break;
                    case 4: val = (seq_t)1255; break;
                    default: val = (seq_t)0; /* unreachable */
                }
            } else {
                if (sizeof(seq_t) <= sizeof(unsigned long long))
                    val = compute_term_small(i, tmp);
                else
                    val = compute_term_big(i, tmp);
            }
            tmp[i] = val;
        }
        cache = tmp;
        cap   = newcap;
    }
    return cache[n];
}
