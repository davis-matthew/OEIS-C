/*
 * A350641  Numbers k such that the product of k and all terms < k in A050376
 *          has more divisors than the product of all terms < k in A050376 and
 *          the smallest term > k in A050376.
 * https://oeis.org/A350641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that d(k*P(k)) > d(P(k)*q(k)), where P(k) =
 *            product of terms < k in A050376 and q(k) = smallest term > k in
 *            A050376; found by direct search using prime exponent counts
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static uint64_t A350641_exp_in(uint64_t p, uint64_t k) {
    uint64_t e = 0, t = 1, pw = p;
    while (pw < k) {
        e += t;
        t *= 2;
        pw = pw * pw;
    }
    return e;
}

/* returns true if m = r^(2^j), r prime; sets r, and tt = 2^j */
static bool A350641_is_fd(uint64_t m, uint64_t *r, uint64_t *tt) {
    if (m < 2) return false;
    uint64_t p = 0;
    uint64_t x = m;
    for (uint64_t d = 2; d * d <= x; d++) {
        if (x % d == 0) { p = d; break; }
    }
    if (p == 0) { *r = m; *tt = 1; return true; }
    uint64_t cnt = 0;
    while (x % p == 0) { x /= p; cnt++; }
    if (x != 1) return false;
    if (cnt & (cnt - 1)) return false;
    *r = p;
    *tt = cnt;
    return true;
}

static bool A350641_test(uint64_t k) {
    /* ratio for k */
    unsigned __int128 Nk = 1, Dk = 1;
    uint64_t x = k;
    for (uint64_t p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            uint64_t v = 0;
            while (x % p == 0) { x /= p; v++; }
            uint64_t e = A350641_exp_in(p, k);
            Nk *= (e + v + 1);
            Dk *= (e + 1);
        }
    }
    if (x > 1) {
        uint64_t e = A350641_exp_in(x, k);
        Nk *= (e + 2);
        Dk *= (e + 1);
    }
    /* next FD prime > k */
    uint64_t r = 0, tt = 0, m = k + 1;
    while (!A350641_is_fd(m, &r, &tt)) m++;
    uint64_t e = A350641_exp_in(r, k);
    unsigned __int128 Nq = e + tt + 1, Dq = e + 1;
    return Nk * Dq > Nq * Dk;
}

static uint64_t *A350641_cache = NULL;
static size_t A350641_cnt = 0, A350641_cap = 0;
static uint64_t A350641_next = 2;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    while (A350641_cnt < (size_t)n) {
        while (!A350641_test(A350641_next)) A350641_next++;
        if (A350641_cnt == A350641_cap) {
            A350641_cap = A350641_cap ? A350641_cap * 2 : 64;
            A350641_cache = (uint64_t *)realloc(A350641_cache, A350641_cap * sizeof(uint64_t));
        }
        A350641_cache[A350641_cnt++] = A350641_next;
        A350641_next++;
    }
    return (seq_t)A350641_cache[(size_t)n - 1];
}
