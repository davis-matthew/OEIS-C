/*
 * A270659  Maximal value of A270655 between indices 3^n and 3^(n+1).
 * https://oeis.org/A270659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = max{ A270655(i) : 3^n <= i <= 3^(n+1) }, where
 *            A270655(3k)=A270655(k), A270655(3k+1)=A270655(3k)+A270655(3k+3),
 *            A270655(3k+2)=A270655(3k+3)-A270655(3k), A270655(0)=0,
 *            A270655(1)=1
 * Range:     u8 n <= 12, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    size_t N = 1;
    for (idx_t k = 0; k < n; k++) N *= 3;   /* N = 3^n */
    int32_t *f = (int32_t *)malloc((N + 2) * sizeof(int32_t));
    if (!f) exit(EXIT_FAILURE);
    f[0] = 0; f[1] = 1;
    for (size_t i = 2; i <= N + 1; i++) {
        size_t r = i % 3;
        if (r == 0) f[i] = f[i / 3];
        else if (r == 1) f[i] = f[i - 1] + f[(i + 2) / 3];
        else f[i] = f[(i + 1) / 3] - f[i - 2];
    }
    long long best = 1; /* f(3^(n+1)) = 1 */
    for (size_t m = N / 3; m < N; m++) {
        long long x = f[m], y = f[m + 1];
        if (x > best) best = x;
        if (x + y > best) best = x + y;
        if (y - x > best) best = y - x;
    }
    free(f);
    return (seq_t)best;
}
