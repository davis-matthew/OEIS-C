/*
 * A243344  a(1) = 1, a(2n) = A013929(a(n)), a(2n+1) = A005117(1+a(n)).
 * https://oeis.org/A243344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2n) = A013929(a(n)), a(2n+1) = A005117(1+a(n));
 *            computed from the binary digits of n, with the k-th
 *            (non)squarefree number found by binary search on a Moebius-sum
 *            count
 * Range:     u8 n <= 63, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Validated: standard, pass, 81 terms through n = 4016 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* number of squarefree integers <= x, given mu[1..r] with r >= floor(sqrt(x)) */
static uint64_t a243344_sqf_count(uint64_t x, const int *mu)
{
    int64_t s = 0;
    for (uint64_t i = 1; i * i <= x; i++) {
        if (mu[i]) s += (int64_t)mu[i] * (int64_t)(x / (i * i));
    }
    return (uint64_t)s;
}

/* k-th squarefree (want_sqf=1) or k-th non-squarefree (want_sqf=0) number */
static uint64_t a243344_kth(uint64_t k, int want_sqf)
{
    uint64_t hi = 4 * k + 10, lo = 1;
    uint64_t r = 1;
    while (r * r <= hi) r++;
    r++;
    int *mu = (int *)malloc((size_t)(r + 1) * sizeof(int));
    char *comp = (char *)calloc((size_t)(r + 1), 1);
    for (uint64_t i = 0; i <= r; i++) mu[i] = 1;
    for (uint64_t p = 2; p <= r; p++) {
        if (comp[p]) continue;
        for (uint64_t m = p; m <= r; m += p) {
            if (m > p) comp[m] = 1;
            mu[m] = -mu[m];
        }
        for (uint64_t m = p * p; m <= r; m += p * p) mu[m] = 0;
    }
    mu[0] = 0;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        uint64_t q = a243344_sqf_count(mid, mu);
        uint64_t c = want_sqf ? q : mid - q;
        if (c >= k) hi = mid;
        else lo = mid + 1;
    }
    free(mu);
    free(comp);
    return lo;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    int top = 0;
    while (top < 63 && ((n >> (top + 1)) != 0)) top++;
    uint64_t v = 1;
    for (int b = top - 1; b >= 0; b--) {
        if ((n >> b) & 1u)
            v = a243344_kth(v + 1, 1);
        else
            v = a243344_kth(v, 0);
    }
    return (seq_t)v;
}
