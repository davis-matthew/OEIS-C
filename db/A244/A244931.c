/*
 * A244931  Prime terms in A214831.
 * https://oeis.org/A244931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th prime among A214831(k), where A214831(k) =
 *            A214831(k-1) + A214831(k-2) + A214831(k-3), A214831(0) = 1,
 *            A214831(1) = A214831(2) = 9; primality by Miller-Rabin
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A244931_found = NULL;
static size_t A244931_cnt = 0;
static size_t A244931_cap = 0;
static bool A244931_init = false;
static seq_t A244931_p, A244931_q, A244931_r;
static int A244931_width = -1;

/* Bit width of seq_t (1024 means effectively unbounded). */
static int A244931_seq_width(void)
{
    if (A244931_width < 0) {
        seq_t x = (seq_t)1;
        int w = 0;
        while (x != 0 && w < 1024) {
            x <<= 1;
            w++;
        }
        A244931_width = w;
    }
    return A244931_width;
}

static int A244931_bitlen(seq_t x)
{
    int b = 0;
    while (x != 0) {
        x >>= 1;
        b++;
    }
    return b;
}

/* Overflow-free modular multiplication over seq_t. */
static seq_t A244931_mulmod(seq_t x, seq_t y, seq_t m, bool direct)
{
    if (direct) return (x * y) % m;
    seq_t r = (seq_t)0;
    x %= m;
    y %= m;
    while (y != 0) {
        if ((y & 1) != 0) {
            seq_t t = m - x;
            if (r >= t) r -= t;
            else r += x;
        }
        {
            seq_t t = m - x;
            if (x >= t) x -= t;
            else x += x;
        }
        y >>= 1;
    }
    return r;
}

static seq_t A244931_powmod(seq_t b, seq_t e, seq_t m, bool direct)
{
    seq_t r = (seq_t)1 % m;
    b %= m;
    while (e != 0) {
        if ((e & 1) != 0) r = A244931_mulmod(r, b, m, direct);
        e >>= 1;
        if (e != 0) b = A244931_mulmod(b, b, m, direct);
    }
    return r;
}

static bool A244931_is_prime(seq_t n)
{
    static const unsigned small[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31,
                                     37, 41, 43, 47, 53, 59, 61, 67, 71};
    const int ns = (int)(sizeof(small) / sizeof(small[0]));
    int i;
    if (n < 2) return false;
    for (i = 0; i < ns; i++) {
        if (n == (seq_t)small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    {
        static const unsigned tp[] = {73, 79, 83, 89, 97, 101, 103, 107, 109,
                                      113, 127, 131, 137, 139, 149, 151, 157,
                                      163, 167, 173, 179, 181, 191, 193, 197, 199};
        int nt = (int)(sizeof(tp) / sizeof(tp[0]));
        for (i = 0; i < nt; i++) {
            if (n == (seq_t)tp[i]) return true;
            if (n % tp[i] == 0) return false;
        }
    }
    bool direct = 2 * A244931_bitlen(n) <= A244931_seq_width();
    seq_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }
    for (i = 0; i < ns; i++) {
        seq_t x = A244931_powmod((seq_t)small[i], d, n, direct);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        int r;
        for (r = 1; r < s; r++) {
            x = A244931_mulmod(x, x, n, direct);
            if (x == n - 1) {
                comp = false;
                break;
            }
        }
        if (comp) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (!A244931_init) {
        A244931_p = (seq_t)1;
        A244931_q = (seq_t)9;
        A244931_r = (seq_t)9;
        A244931_init = true;
    }
    while (A244931_cnt < (size_t)n) {
        if (A244931_is_prime(A244931_p)) {
            if (A244931_cnt == A244931_cap) {
                size_t nc = A244931_cap ? A244931_cap * 2 : 16;
                A244931_found = mem_resize(seq_t, A244931_found, nc);
                A244931_cap = nc;
            }
            A244931_found[A244931_cnt++] = A244931_p;
        }
        seq_t s = A244931_p + A244931_q + A244931_r;
        A244931_p = A244931_q;
        A244931_q = A244931_r;
        A244931_r = s;
    }
    return A244931_found[(size_t)n - 1];
}
