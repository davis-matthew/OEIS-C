/*
 * A366803  Dirichlet convolution of Liouville's lambda with A342001 applied
 *          to Doudna sequence: a(n) = A347395(A005940(1+n)).
 * https://oeis.org/A366803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d | m} (-1)^bigomega(m/d) * A342001(d), where m =
 *            A005940(n+1) (Doudna sequence), i.e. A347395(m). Computed by
 *            enumerating divisors from the Doudna exponent vector
 * Range:     u8 n <= 83, u16 n <= 2723, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 109 terms through n = 9004 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    int ex[70];
    int i;
    for (i = 0; i < 70; i++) ex[i] = 0;

    /* small primes */
    static int pr[70];
    static int have = 0;
    if (!have) {
        int c = 0, x = 2;
        while (c < 70) {
            int ok = 1;
            for (int j = 0; j < c; j++) {
                if (pr[j] * pr[j] > x) break;
                if (x % pr[j] == 0) { ok = 0; break; }
            }
            if (ok) pr[c++] = x;
            x++;
        }
        have = 1;
    }

    /* Doudna: exponent of prime(k) = number of 1 bits followed by k-1 zeros */
    idx_t m = n;
    int k = 1;
    while (m > 0) {
        if (m & 1) { ex[k - 1]++; m = (m - 1) / 2; }
        else { m /= 2; k++; }
    }

    int P[70], E[70], f[70];
    int cnt = 0;
    int total = 0;
    for (i = 0; i < 70; i++) {
        if (ex[i] > 0) {
            P[cnt] = pr[i];
            E[cnt] = ex[i];
            total += ex[i];
            cnt++;
        }
    }
    for (i = 0; i < cnt; i++) f[i] = 0;

    seq_t pos = 0, neg = 0;
    for (;;) {
        /* leaf */
        seq_t rad = 1;
        int fs = 0;
        for (i = 0; i < cnt; i++) {
            if (f[i] > 0) rad *= (seq_t)P[i];
            fs += f[i];
        }
        seq_t val = 0;
        for (i = 0; i < cnt; i++) {
            if (f[i] > 0) val += (seq_t)f[i] * (rad / (seq_t)P[i]);
        }
        if (((total - fs) & 1) == 0) pos += val;
        else neg += val;

        /* advance odometer */
        int j = 0;
        while (j < cnt) {
            if (f[j] < E[j]) { f[j]++; break; }
            f[j] = 0;
            j++;
        }
        if (j >= cnt) break;
    }
    return pos - neg;
}
