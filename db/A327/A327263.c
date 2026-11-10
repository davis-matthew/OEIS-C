/*
 * A327263  Array T(n,k) in which the i-th row consists of numbers > 1 not in
 *          array U(i;n,k) = (i*n*k - (i-2)*A319929(n,k))/2 where i >= 1, n >=
 *          1 and k >= 1, read by antidiagonals.
 * https://oeis.org/A327263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   row i (i>=1) lists the numbers m>1 not of the form U(i;n,k) =
 *            (i*n*k - (i-2)*A319929(n,k))/2 with n,k>=2; the array is read by
 *            antidiagonals, a(n) = T(i,j) with i+j-1 = d the antidiagonal.
 *            Computed by a sieve with a doubling limit
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static unsigned long long A327263_u(unsigned long long i, unsigned long long n, unsigned long long k)
{
    unsigned long long a;
    if (n & 1ULL) {
        if (k & 1ULL) a = n + k - 1;
        else a = k;
    } else {
        if (k & 1ULL) a = n;
        else a = 0;
    }
    if (i == 1) return (n * k + a) / 2;
    return (i * n * k - (i - 2) * a) / 2;
}

seq_t a(idx_t n)
{
    unsigned long long d = 1;
    while ((unsigned long long)d * (d + 1) / 2 < (unsigned long long)n) d++;
    unsigned long long p = (unsigned long long)n - d * (d - 1) / 2; /* 1..d */
    unsigned long long row = p;
    unsigned long long col = d + 1 - p;

    unsigned long long L = 64;
    for (;;) {
        unsigned char *mark = (unsigned char *)calloc((size_t)(L + 1), 1);
        if (!mark) return (seq_t)0;
        for (unsigned long long x = 2; x * x <= 2 * L; x++) {
            for (unsigned long long y = x; x * y <= 2 * L; y++) {
                unsigned long long u = A327263_u(row, x, y);
                if (u <= L) mark[u] = 1;
            }
        }
        unsigned long long cnt = 0, res = 0;
        for (unsigned long long m = 2; m <= L; m++) {
            if (!mark[m]) {
                cnt++;
                if (cnt == col) { res = m; break; }
            }
        }
        free(mark);
        if (res) return (seq_t)res;
        L *= 2;
    }
}
