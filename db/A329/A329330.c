/*
 * A329330  Multiplication operation of a ring over the positive integers that
 *          has A059897(.,.) as addition operation and is isomorphic to
 *          GF(2)[x] with polynomial x^i mapped to A050376(i+1). Square array
 *          read by descending antidiagonals: A(n,k), n >= 1, k >= 1.
 * https://oeis.org/A329330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = A052330(A048720(A052331(n), A052331(k))), read by
 *            descending antidiagonals
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-08)
 * Helpers:   is_prime, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/isqrt.h"

#define FD_MAX 4096
static idx_t fd_list[FD_MAX];
static size_t fd_cnt = 0;
static idx_t fd_next = 2;

/* is m a Fermi-Dirac prime p^(2^k)? */
static bool fd_is(idx_t m) {
    if (m < 2) return false;
    if (is_prime(m)) return true;
    idx_t r = isqrt(m);
    if (r * r == m) return fd_is(r);
    return false;
}

static void fd_ensure_count(size_t c) {
    if (c > FD_MAX) c = FD_MAX;
    while (fd_cnt < c) {
        if (fd_is(fd_next)) fd_list[fd_cnt++] = fd_next;
        fd_next++;
    }
}

static void fd_ensure_value(idx_t v) {
    while (fd_next <= v && fd_cnt < FD_MAX) {
        if (fd_is(fd_next)) fd_list[fd_cnt++] = fd_next;
        fd_next++;
    }
}

static int fd_index(idx_t q) {
    for (size_t t = 0; t < fd_cnt; ++t)
        if (fd_list[t] == q) return (int)t;
    return -1;
}

/* A052331: bitmask of Fermi-Dirac prime indices in the factorization of m */
static unsigned __int128 to_mask(idx_t m) {
    unsigned __int128 mask = 0;
    fd_ensure_value(m);
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p) continue;
        idx_t e = 0;
        while (m % p == 0) { m /= p; ++e; }
        idx_t q = p;
        while (e) {
            if (e & 1) {
                int t = fd_index(q);
                if (t >= 0 && t < 128) mask |= ((unsigned __int128)1) << t;
            }
            e >>= 1;
            if (e) q = q * q;
        }
    }
    if (m > 1) {
        int t = fd_index(m);
        if (t >= 0 && t < 128) mask |= ((unsigned __int128)1) << t;
    }
    return mask;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    idx_t d = (isqrt(8 * n - 7) + 1) / 2;
    idx_t pos = n - d * (d - 1) / 2;
    idx_t i = pos, j = d + 1 - pos;

    unsigned __int128 u = to_mask(i), v = to_mask(j);
    unsigned __int128 prod = 0;
    for (int b = 0; b < 128; ++b)
        if ((v >> b) & 1) prod ^= (u << b);

    int top = 0;
    for (int b = 0; b < 128; ++b)
        if ((prod >> b) & 1) top = b + 1;
    fd_ensure_count((size_t)top);

    seq_t res = 1;
    for (int b = 0; b < top; ++b)
        if ((prod >> b) & 1) res *= (seq_t)fd_list[b];
    return res;
}
