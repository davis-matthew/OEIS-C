/*
 * A308961  Numbers k not divisible by 10 such that k^2 and 2*k are in
 *          A046030.
 * https://oeis.org/A308961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k, 10 not dividing k, with k^2 and 2k in A046030
 *            (all decimal digits in {0,1,4,9}); computed by a digit-by-digit
 *            suffix search, lengths increasing
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A308961_MAXD 120

static int A308961_d[A308961_MAXD];
static int A308961_car[2 * A308961_MAXD];
static int A308961_c2[A308961_MAXD];

static int A308961_ok(int x)
{
    return x == 0 || x == 1 || x == 4 || x == 9;
}

static int A308961_final(int L)
{
    int c = A308961_car[L];
    int e = A308961_c2[L];
    if (e > 0 && !A308961_ok(e))
        return 0;
    for (int col = L; col <= 2 * L - 2; col++) {
        int sum = 0;
        for (int i = col - (L - 1); i <= L - 1; i++)
            sum += A308961_d[i] * A308961_d[col - i];
        int t = sum + c;
        if (!A308961_ok(t % 10))
            return 0;
        c = t / 10;
    }
    while (c > 0) {
        if (!A308961_ok(c % 10))
            return 0;
        c /= 10;
    }
    return 1;
}

static void A308961_dfs(int m, int D, seq_t *found, int *nf)
{
    for (int x = (m == 0 ? 1 : 0); x <= 9; x++) {
        if (m == D - 1 && x == 0)
            continue;
        A308961_d[m] = x;
        int sum = 0;
        for (int i = 0; i <= m; i++)
            sum += A308961_d[i] * A308961_d[m - i];
        int t = sum + A308961_car[m];
        if (!A308961_ok(t % 10))
            continue;
        int u = 2 * x + A308961_c2[m];
        if (!A308961_ok(u % 10))
            continue;
        A308961_car[m + 1] = t / 10;
        A308961_c2[m + 1] = u / 10;
        if (m == D - 1) {
            if (A308961_final(D) && *nf < 64) {
                seq_t v = 0;
                for (int i = D - 1; i >= 0; i--)
                    v = v * 10 + (seq_t)A308961_d[i];
                found[(*nf)++] = v;
            }
        } else {
            A308961_dfs(m + 1, D, found, nf);
        }
    }
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    A308961_car[0] = 0;
    A308961_c2[0] = 0;
    for (int D = 1; D < A308961_MAXD; D++) {
        seq_t found[64];
        int nf = 0;
        A308961_dfs(0, D, found, &nf);
        for (int i = 1; i < nf; i++) {
            seq_t v = found[i];
            int j = i - 1;
            while (j >= 0 && found[j] > v) {
                found[j + 1] = found[j];
                j--;
            }
            found[j + 1] = v;
        }
        if (count + (idx_t)nf >= n)
            return found[(int)(n - count - 1)];
        count += (idx_t)nf;
    }
    return 0;
}
