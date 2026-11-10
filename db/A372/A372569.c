/*
 * A372569  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A009194(i) = A009194(j), A009195(i) = A009195(j) and A009223(i)
 *          = A009223(j), for all i, j >= 1.
 * https://oeis.org/A372569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the triple
 *            [gcd(n,sigma(n)), gcd(n,phi(n)), gcd(sigma(n),phi(n))]; computed
 *            incrementally by first-appearance numbering
 * Range:     u8 n <= 247, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 119 terms through n = 9098 (2026-11-10)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static void A372569_triple(idx_t m, seq_t t[3])
{
    seq_t sg = 1, ph = 1;
    idx_t x = m;
    for (idx_t p = 2; p * p <= x; p += (p == 2 ? 1 : 2)) {
        if (x % p == 0) {
            seq_t sum = 1, pw = 1;
            seq_t pp = (seq_t)p;
            while (x % p == 0) {
                x /= p;
                pw *= pp;
                sum += pw;
            }
            sg *= sum;
            ph *= (pw / pp) * (pp - 1);
        }
    }
    if (x > 1) {
        seq_t pp = (seq_t)x;
        sg *= (pp + 1);
        ph *= (pp - 1);
    }
    seq_t mm = (seq_t)m;
    t[0] = gcd(mm, sg);
    t[1] = gcd(mm, ph);
    t[2] = gcd(sg, ph);
}

static seq_t *A372569_ids = 0;
static size_t A372569_idcap = 0;
static seq_t *A372569_tr = 0;
static size_t A372569_trcap = 0;
static size_t A372569_ndist = 0;
static idx_t A372569_done = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if ((size_t)n > A372569_idcap) {
        size_t nc = A372569_idcap ? A372569_idcap : 1024;
        while (nc < (size_t)n) nc *= 2;
        if (A372569_ids == 0) A372569_ids = mem_new(seq_t, nc);
        else A372569_ids = mem_resize(seq_t, A372569_ids, nc);
        A372569_idcap = nc;
    }
    while (A372569_done < n) {
        idx_t m = A372569_done + 1;
        seq_t t[3];
        A372569_triple(m, t);
        size_t found = 0;
        for (size_t k = 0; k < A372569_ndist; ++k) {
            const seq_t *e = A372569_tr + 3 * k;
            if (e[0] == t[0] && e[1] == t[1] && e[2] == t[2]) {
                found = k + 1;
                break;
            }
        }
        if (!found) {
            if (3 * (A372569_ndist + 1) > A372569_trcap) {
                size_t nc = A372569_trcap ? A372569_trcap * 2 : 3072;
                if (A372569_tr == 0) A372569_tr = mem_new(seq_t, nc);
                else A372569_tr = mem_resize(seq_t, A372569_tr, nc);
                A372569_trcap = nc;
            }
            seq_t *e = A372569_tr + 3 * A372569_ndist;
            e[0] = t[0]; e[1] = t[1]; e[2] = t[2];
            ++A372569_ndist;
            found = A372569_ndist;
        }
        A372569_ids[m - 1] = (seq_t)found;
        A372569_done = m;
    }
    return A372569_ids[n - 1];
}
