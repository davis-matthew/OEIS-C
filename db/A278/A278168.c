/*
 * A278168  a(n) = number of integers one less than a prime encountered before
 *          reaching 0 when starting from k = ((n+1)^2)-1 and iterating map k
 *          -> k - A002828(k).
 * https://oeis.org/A278168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c((n+1)^2-1), where c(0) = 0 and c(k) = [k+1 is prime] +
 *            c(k - A002828(k)) for k > 0. Computed with a sieve: A002828 is
 *            found by classifying sums of squares, primes by an Eratosthenes
 *            sieve, then c is filled in for increasing k
 * Range:     u8 n <= 72, u16 n <= 1685, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 7556 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *a278168_vals = 0;
static uint64_t a278168_cached = 0;

static void a278168_build(uint64_t N)
{
    uint64_t K = (N + 1) * (N + 1) - 1;
    uint64_t size = K + 2, i, j, p, x, y, k, c;
    uint64_t win[8];
    uint64_t *vals;
    uint8_t *cls = (uint8_t *)malloc((size_t)size);
    if (!cls) return;
    vals = (uint64_t *)malloc((size_t)((N + 1) * sizeof(uint64_t)));
    if (!vals) { free(cls); return; }

    /* low 4 bits: A002828(k); bit 4: k is not prime */
    for (i = 0; i < size; i++) cls[i] = 3;
    for (p = 1; p <= K; p *= 4)
        for (j = 7 * p; j <= K; j += 8 * p) cls[j] = 4;
    for (x = 1; 2 * x * x <= K; x++)
        for (y = x; x * x + y * y <= K; y++) cls[x * x + y * y] = 2;
    for (x = 1; x * x <= K; x++) cls[x * x] = 1;
    cls[0] = 0;

    cls[0] |= 16;
    cls[1] |= 16;
    for (i = 2; i * i <= K + 1; i++)
        if (!(cls[i] & 16))
            for (j = i * i; j <= K + 1; j += i) cls[j] |= 16;

    for (i = 0; i < 8; i++) win[i] = 0;
    {
        uint64_t nextj = 1;
        vals[0] = 0;
        for (k = 0; k <= K; k++) {
            if (k == 0) c = 0;
            else {
                uint64_t s = cls[k] & 15;
                c = (uint64_t)((cls[k + 1] & 16) ? 0 : 1) + win[(k - s) & 7];
            }
            win[k & 7] = c;
            if (nextj <= N && k == (nextj + 1) * (nextj + 1) - 1) {
                vals[nextj] = c;
                nextj++;
            }
        }
    }
    free(cls);
    if (a278168_vals) free(a278168_vals);
    a278168_vals = vals;
    a278168_cached = N;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    if (m == 0) return (seq_t)0;
    if (m > a278168_cached) {
        uint64_t target = m;
        if (a278168_cached * 3 / 2 > target) target = a278168_cached * 3 / 2;
        if (target < 100) target = 100;
        if (target > 12000 && m <= 12000) target = 12000;
        a278168_build(target);
        if (m > a278168_cached) return (seq_t)0;
    }
    return (seq_t)a278168_vals[m];
}
