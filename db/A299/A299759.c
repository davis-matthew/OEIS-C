/*
 * A299759  Triangle read by rows in which row n lists in order all FDH
 *          numbers of strict integer partitions of n.
 * https://oeis.org/A299759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   row r lists, in increasing order, the FDH numbers prod f(s_i) of
 *            strict partitions (s_1>...>s_k) of r, f = A050376 (Fermi-Dirac
 *            primes); row 0 = {1}. Computed by enumerating and sorting each
 *            row
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

#define FD_MAXR 300
#define FD_LIM 5000

static uint64_t fd_q[FD_MAXR + 2];
static uint64_t fd_f[FD_MAXR + 2];
static bool fd_init = false;

static seq_t *fd_row = NULL;
static size_t fd_row_n = 0, fd_row_cap = 0;
static long long fd_row_r = -1;

static bool fd_isfd(unsigned v) {
    if (v < 2) return false;
    bool prime = true;
    for (unsigned d = 2; d * d <= v; d++) if (v % d == 0) { prime = false; break; }
    if (prime) return true;
    unsigned r = 0;
    while (r * r < v) r++;
    if (r * r == v) return fd_isfd(r);
    return false;
}

static void fd_setup(void) {
    if (fd_init) return;
    fd_init = true;
    size_t c = 0;
    for (unsigned v = 2; v <= FD_LIM && c < FD_MAXR + 1; v++)
        if (fd_isfd(v)) fd_f[++c] = v;
    for (int i = 0; i <= FD_MAXR + 1; i++) fd_q[i] = 0;
    fd_q[0] = 1;
    for (int p = 1; p <= FD_MAXR; p++)
        for (int j = FD_MAXR; j >= p; j--) fd_q[j] += fd_q[j - p];
}

static void fd_push(seq_t v) {
    if (fd_row_n == fd_row_cap) {
        size_t nc = fd_row_cap ? fd_row_cap * 2 : 64;
        if (!fd_row) fd_row = mem_new(seq_t, nc);
        else fd_row = mem_resize(seq_t, fd_row, nc);
        fd_row_cap = nc;
    }
    fd_row[fd_row_n++] = v;
}

static void fd_rec(int rem, int maxp, seq_t prod) {
    if (rem == 0) { fd_push(prod); return; }
    int top = rem < maxp ? rem : maxp;
    for (int p = top; p >= 1; p--)
        fd_rec(rem - p, p - 1, prod * (seq_t)fd_f[p]);
}

static void fd_sort(void) {
    size_t n = fd_row_n;
    for (size_t gap = n / 2; gap > 0; gap /= 2) {
        for (size_t i = gap; i < n; i++) {
            seq_t t = fd_row[i];
            size_t j = i;
            while (j >= gap && fd_row[j - gap] > t) {
                fd_row[j] = fd_row[j - gap];
                j -= gap;
            }
            fd_row[j] = t;
        }
    }
}

seq_t a(idx_t n) {
    fd_setup();
    if (n == 0) return 0;
    idx_t k = n - 1;
    int r = 0;
    while (r <= FD_MAXR && k >= fd_q[r]) { k -= fd_q[r]; r++; }
    if (r > FD_MAXR) return 0;
    if (fd_row_r != r) {
        fd_row_n = 0;
        fd_rec(r, r, (seq_t)1);
        fd_sort();
        fd_row_r = r;
    }
    return fd_row[(size_t)k];
}
