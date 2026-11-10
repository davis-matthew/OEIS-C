/*
 * A292258  a(1) = 1; for n > 1, a(n) = prime(A101296(n)-1) * a(floor(n/2)).
 * https://oeis.org/A292258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = prime(A101296(n)-1) * a(floor(n/2)) for n > 1.
 *            A101296(n) = number of terms of A025487 (products of primorials)
 *            <= least representative of the prime signature of n
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 383, u64 n <= 6143,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 80 terms through n = 7733 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const uint64_t A292258_pr[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};

static uint64_t A292258_cnt(uint64_t lim, int pi, int maxe, uint64_t cur) {
    uint64_t c = 1, v = cur;
    if (pi >= 16) return 1;
    for (int e = 1; e <= maxe; e++) {
        if (v > lim / A292258_pr[pi]) break;
        v *= A292258_pr[pi];
        c += A292258_cnt(lim, pi + 1, e, v);
    }
    return c;
}

static uint64_t A292258_rank(uint64_t n) {
    int ex[16], len = 0;
    uint64_t x = n;
    for (uint64_t p = 2; p * p <= x; p++) {
        int c = 0;
        while (x % p == 0) { c++; x /= p; }
        if (c) ex[len++] = c;
    }
    if (x > 1) ex[len++] = 1;
    for (int i = 1; i < len; i++) {
        int key = ex[i], j = i;
        while (j > 0 && ex[j-1] < key) { ex[j] = ex[j-1]; j--; }
        ex[j] = key;
    }
    uint64_t rep = 1;
    for (int i = 0; i < len; i++)
        for (int e = 0; e < ex[i]; e++) rep *= A292258_pr[i];
    return A292258_cnt(rep, 0, 64, 1);
}

static uint64_t A292258_nthprime(uint64_t k) {
    uint64_t c = 0, p = 1;
    while (c < k) {
        p++;
        bool ok = true;
        for (uint64_t d = 2; d * d <= p; d++)
            if (p % d == 0) { ok = false; break; }
        if (ok) c++;
    }
    return p;
}

seq_t a(idx_t n) {
    seq_t res = 1;
    uint64_t m = (uint64_t)n;
    while (m > 1) {
        uint64_t r = A292258_rank(m);
        res *= (seq_t)A292258_nthprime(r - 1);
        m /= 2;
    }
    return res;
}
