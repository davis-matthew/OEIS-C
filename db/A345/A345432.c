/*
 * A345432  For 1<=x<=n, 1<=y<=n, write gcd(x,y) = u*x+v*y with u,v minimal;
 *          a(n) = sum of the values of |u|.
 * https://oeis.org/A345432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{1<=x,y<=n} |u|, where gcd(x,y) = u*x+v*y with u,v
 *            chosen to minimize u^2+v^2. For x=y the special rule u=0,v=1 is
 *            used
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   _egcd, _ugcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/_egcd.h"
#include "../utils/_ugcd.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i + 1; j <= n; ++j) {
            idx_t g = _ugcd(i, j);
            /* Reduce to coprime pair */
            int X = (int)(i / g);
            int Y = (int)(j / g);

            long long u, v;
            _egcd(X, Y, &u, &v);

            long long absu = u >= 0 ? u : -u;
            long long absv = v >= 0 ? v : -v;

            total += (seq_t)absu + (seq_t)absv;
        }
    }
    return total;
}
