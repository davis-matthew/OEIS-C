/*
 * A336580  Numbers k such that A334676(k) = 1.
 * https://oeis.org/A336580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th number k such that A334676(k) = 1, i.e. 1 is
 *            reachable from k by repeatedly dividing by a nonzero digit of
 *            the current number that divides it. Computed by search over the
 *            7-smooth numbers in increasing order (all terms are 7-smooth)
 * Range:     u8 n <= 27, u16 n <= 158, u32 n <= 3411, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9866 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A336580_sm = NULL;
static idx_t A336580_sm_cap = 0, A336580_sm_len = 0;
static idx_t A336580_i2 = 0, A336580_i3 = 0, A336580_i5 = 0, A336580_i7 = 0;

static seq_t *A336580_found = NULL;
static idx_t A336580_f_cap = 0, A336580_f_len = 0;

static seq_t *A336580_vis = NULL;
static idx_t A336580_v_cap = 0, A336580_v_len = 0;

static bool A336580_reach(seq_t x)
{
    if (x == 1) return true;
    for (idx_t i = 0; i < A336580_v_len; i++)
        if (A336580_vis[i] == x) return false;
    if (A336580_v_len >= A336580_v_cap) {
        idx_t nc = A336580_v_cap * 2;
        seq_t *nv = mem_new(seq_t, nc);
        for (idx_t i = 0; i < A336580_v_len; i++) nv[i] = A336580_vis[i];
        mem_free(A336580_vis);
        A336580_vis = nv;
        A336580_v_cap = nc;
    }
    A336580_vis[A336580_v_len++] = x;

    bool seen[10] = {false, false, false, false, false, false, false, false, false, false};
    seq_t t = x;
    while (t > 0) {
        int d = (int)(t % 10);
        t /= 10;
        if (d >= 2 && !seen[d]) {
            seen[d] = true;
            if (x % (seq_t)d == 0) {
                if (A336580_reach(x / (seq_t)d)) return true;
            }
        }
    }
    return false;
}

static bool A336580_test(seq_t x)
{
    if (A336580_vis == NULL) {
        A336580_v_cap = 256;
        A336580_vis = mem_new(seq_t, A336580_v_cap);
    }
    A336580_v_len = 0;
    return A336580_reach(x);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (A336580_sm == NULL) {
        A336580_sm_cap = 1024;
        A336580_sm = mem_new(seq_t, A336580_sm_cap);
        A336580_sm[0] = 1;
        A336580_sm_len = 1;
        A336580_f_cap = 256;
        A336580_found = mem_new(seq_t, A336580_f_cap);
        A336580_found[0] = 1;   /* 1 is a term (A334676(1)=1) */
        A336580_f_len = 1;
    }
    while (A336580_f_len < n) {
        seq_t n2 = A336580_sm[A336580_i2] * (seq_t)2;
        seq_t n3 = A336580_sm[A336580_i3] * (seq_t)3;
        seq_t n5 = A336580_sm[A336580_i5] * (seq_t)5;
        seq_t n7 = A336580_sm[A336580_i7] * (seq_t)7;
        seq_t nx = n2;
        if (n3 < nx) nx = n3;
        if (n5 < nx) nx = n5;
        if (n7 < nx) nx = n7;
        if (nx == n2) A336580_i2++;
        if (nx == n3) A336580_i3++;
        if (nx == n5) A336580_i5++;
        if (nx == n7) A336580_i7++;

        if (A336580_sm_len >= A336580_sm_cap) {
            idx_t nc = A336580_sm_cap * 2;
            seq_t *ns = mem_new(seq_t, nc);
            for (idx_t i = 0; i < A336580_sm_len; i++) ns[i] = A336580_sm[i];
            mem_free(A336580_sm);
            A336580_sm = ns;
            A336580_sm_cap = nc;
        }
        A336580_sm[A336580_sm_len++] = nx;

        if (A336580_test(nx)) {
            if (A336580_f_len >= A336580_f_cap) {
                idx_t nc = A336580_f_cap * 2;
                seq_t *nf = mem_new(seq_t, nc);
                for (idx_t i = 0; i < A336580_f_len; i++) nf[i] = A336580_found[i];
                mem_free(A336580_found);
                A336580_found = nf;
                A336580_f_cap = nc;
            }
            A336580_found[A336580_f_len++] = nx;
        }
    }
    return A336580_found[n - 1];
}
