/*
 * A349282  a(n) is the least k such that A349281(k) = n.
 * https://oeis.org/A349282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with A349281(k) = n; computed as min of Prod
 *            p_i^e_i over distinct primes with Sum tau(e_i) = n, by DP over
 *            the first n primes (using the smallest e with tau(e)=t for each
 *            cost t)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 40 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static bool A349282_mul(seq_t x, seq_t y, seq_t *r)
{
    seq_t p = x * y;
    if (y != 0 && p / y != x) return false;
    *r = p;
    return true;
}

static int A349282_tau(int e)
{
    int c = 0;
    for (int d = 1; d <= e; d++)
        if (e % d == 0) c++;
    return c;
}

seq_t a(idx_t n)
{
    int N = (int)n;
    if (N == 0) return 1;

    /* smallest exponent for each divisor count t (0 if none within 300) */
    int *et = (int *)malloc(sizeof(int) * (N + 1));
    for (int t = 0; t <= N; t++) et[t] = 0;
    for (int e = 1; e <= 300; e++) {
        int t = A349282_tau(e);
        if (t <= N && et[t] == 0) et[t] = e;
    }

    seq_t *dp = mem_new(seq_t, N + 1);
    char *ok = (char *)malloc(N + 1);
    for (int s = 0; s <= N; s++) ok[s] = 0;
    dp[0] = 1;
    ok[0] = 1;

    seq_t *pw = mem_new(seq_t, N + 1);
    char *pok = (char *)malloc(N + 1);

    int count = 0;
    for (long long p = 2; count < N; p++) {
        bool prime = true;
        for (long long d = 2; d * d <= p; d++)
            if (p % d == 0) { prime = false; break; }
        if (!prime) continue;
        count++;

        for (int t = 1; t <= N; t++) {
            pok[t] = 0;
            if (et[t] == 0) continue;
            seq_t r = 1;
            bool good = true;
            for (int i = 0; i < et[t]; i++) {
                if (!A349282_mul(r, (seq_t)p, &r)) { good = false; break; }
            }
            if (good) { pw[t] = r; pok[t] = 1; }
        }

        for (int s = N; s >= 1; s--) {
            for (int t = 1; t <= s; t++) {
                if (!pok[t] || !ok[s - t]) continue;
                seq_t c;
                if (!A349282_mul(dp[s - t], pw[t], &c)) continue;
                if (!ok[s] || c < dp[s]) { dp[s] = c; ok[s] = 1; }
            }
        }
    }

    seq_t res = dp[N];
    mem_free(dp);
    mem_free(pw);
    free(ok);
    free(pok);
    free(et);
    return res;
}
