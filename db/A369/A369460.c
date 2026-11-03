/*
 * A369460  Number of representations of 12n-9 as a sum (p*q + p*r + q*r) with
 *          three odd primes p <= q <= r.
 * https://oeis.org/A369460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of 12*n-9 as p*q + p*r + q*r
 *            with odd primes p ≤ q ≤ r
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 9297 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A369460: the number of ways to write 12n - 9 as pq + pr + qr with odd primes
 * p <= q <= r. For each p <= q, r = (12n - 9 - pq)/(p + q) must be an integer
 * prime >= q. The prime table covers 12n and grows with n (the original's
 * stopped at 20000, below the r needed from n = 10000 on). */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *isp = NULL;
static uint64_t lim = 0;

static void build(uint64_t L)
{
    free(isp);
    isp = (unsigned char *)malloc((size_t)L + 1);
    if (!isp) abort();
    memset(isp, 1, (size_t)L + 1);
    isp[0] = isp[1] = 0;
    for (uint64_t i = 2; i * i <= L; ++i)
        if (isp[i])
            for (uint64_t j = i * i; j <= L; j += i) isp[j] = 0;
    lim = L;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    const uint64_t T = 12 * (uint64_t)n - 9;
    if (T > lim) {
        uint64_t L = lim ? 2 * lim : 1 << 16;
        while (L < T) L *= 2;
        build(L);
    }
    uint64_t count = 0;
    for (uint64_t p = 3; p * p <= T; p += 2) {
        if (!isp[p]) continue;
        for (uint64_t q = p; p * q < T; q += 2) {
            if (!isp[q]) continue;
            uint64_t rem = T - p * q;
            if (rem % (p + q)) continue;
            uint64_t r = rem / (p + q);
            if (r >= q && (r & 1) && isp[r]) ++count;
        }
    }
    return (seq_t)count;
}
