/*
 * A257006  Irregular triangle read by rows: period lengths of periods of
 *          primitive Zagier-reduced binary quadratic forms with discriminants
 *          D(n) = A079896(n).
 * https://oeis.org/A257006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n,k) = length of the k-th cycle (nonincreasing order) of
 *            primitive Zagier-reduced forms (a,b,c), a>0,c>0,b>a+c, of
 *            discriminant D(n)=A079896(n), under the map
 *            (a,b,c)->((b'^2-D)/(4a),b',a) with b'=2ak-b, k minimal such that
 *            b'>sqrt(D); computed by enumerating forms and tracing cycles
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static long long A257006_gcd(long long x, long long y)
{
    while (y) { long long t = x % y; x = y; y = t; }
    return x;
}

static bool A257006_issq(long long x)
{
    long long r = 0;
    while (r * r < x) r++;
    return r * r == x;
}

seq_t a(idx_t n)
{
    long long target = (long long)n;
    long long pos = 0;
    for (long long D = 5;; D++) {
        long long m4 = D % 4;
        if (m4 != 0 && m4 != 1) continue;
        if (A257006_issq(D)) continue;

        size_t cap = 64, cnt = 0;
        long long *fa = (long long *)malloc(cap * sizeof(long long));
        long long *fb = (long long *)malloc(cap * sizeof(long long));
        long long *fc = (long long *)malloc(cap * sizeof(long long));
        for (long long b = 1; b <= D + 2; b++) {
            if (((b - D) % 2) != 0) continue;
            long long t = b * b - D;
            if (t <= 0) continue;
            long long N = t / 4;
            for (long long aa = 1; aa < b; aa++) {
                if (N % aa) continue;
                long long cc = N / aa;
                if (aa + cc >= b) continue;
                if (A257006_gcd(A257006_gcd(aa, b), cc) != 1) continue;
                if (cnt == cap) {
                    cap *= 2;
                    fa = (long long *)realloc(fa, cap * sizeof(long long));
                    fb = (long long *)realloc(fb, cap * sizeof(long long));
                    fc = (long long *)realloc(fc, cap * sizeof(long long));
                }
                fa[cnt] = aa; fb[cnt] = b; fc[cnt] = cc; cnt++;
            }
        }
        char *vis = (char *)calloc(cnt + 1, 1);
        long long *lens = (long long *)malloc((cnt + 1) * sizeof(long long));
        size_t nl = 0;
        for (size_t i = 0; i < cnt; i++) {
            if (vis[i]) continue;
            long long len = 0;
            size_t cur = i;
            while (!vis[cur]) {
                vis[cur] = 1;
                len++;
                long long A = fa[cur], B = fb[cur];
                long long k = B / (2 * A);
                long long nb = 2 * A * k - B;
                while (nb <= 0 || nb * nb <= D) { k++; nb = 2 * A * k - B; }
                long long na = (nb * nb - D) / (4 * A);
                size_t j = 0;
                for (; j < cnt; j++)
                    if (fa[j] == na && fb[j] == nb && fc[j] == A) break;
                if (j == cnt) break; /* should not happen */
                cur = j;
            }
            /* insertion, nonincreasing */
            size_t p = nl++;
            while (p > 0 && lens[p - 1] < len) { lens[p] = lens[p - 1]; p--; }
            lens[p] = len;
        }
        seq_t res = 0;
        bool found = false;
        if (pos + (long long)nl >= target) {
            res = (seq_t)lens[target - pos - 1];
            found = true;
        }
        pos += (long long)nl;
        free(fa); free(fb); free(fc); free(vis); free(lens);
        if (found) return res;
    }
}
