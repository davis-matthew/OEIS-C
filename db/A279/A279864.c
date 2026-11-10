/*
 * A279864  Irregular triangle read by rows: the n-th row corresponds to the
 *          natural numbers not exceeding A002110(n) and divisible by the n-th
 *          prime but not by a smaller prime.
 * https://oeis.org/A279864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   row r lists p_r*m for 1<=m<=A002110(r-1) with
 *            gcd(m,A002110(r-1))=1, in increasing order (row length
 *            A005867(r-1)); a(n) is the n-th term over all rows, found by a
 *            direct scan
 * Range:     u8 n <= 18, u16 n <= 1280, u32 n <= 6300, u64 n <= 6300,
 *            u128 n <= 6300, big any n
 * Validated: standard, pass, 67 terms through n = 4728 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static void a279864_primes(uint64_t *pr, int cnt)
{
    int c = 0;
    uint64_t x = 2;
    while (c < cnt) {
        int ok = 1, i;
        for (i = 0; i < c; i++) {
            if (pr[i] * pr[i] > x) break;
            if (x % pr[i] == 0) { ok = 0; break; }
        }
        if (ok) pr[c++] = x;
        x++;
    }
}

seq_t a(idx_t n)
{
    uint64_t pr[40];
    uint64_t rem = (uint64_t)n;
    uint64_t len = 1;      /* length of row r = prod_{k<r}(p_k-1) */
    int r = 1;
    seq_t Q = 1;           /* primorial of r-1 primes */
    seq_t m;
    uint64_t cnt = 0;
    int i;

    a279864_primes(pr, 40);
    while (rem > len) {
        rem -= len;
        len *= (pr[r - 1] - 1);
        Q *= (seq_t)pr[r - 1];
        r++;
    }
    /* find rem-th m >= 1 coprime to the first r-1 primes */
    for (m = 1; ; m++) {
        int ok = 1;
        for (i = 0; i < r - 1; i++) {
            if (m % (seq_t)pr[i] == 0) { ok = 0; break; }
        }
        if (ok) {
            cnt++;
            if (cnt == rem) break;
        }
    }
    (void)Q;
    return (seq_t)pr[r - 1] * m;
}
