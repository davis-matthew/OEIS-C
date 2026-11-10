/*
 * A349896  Record values in A349876.
 * https://oeis.org/A349896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th record value of A349876 (A349876(k) = k if 3|k, else
 *            A349876(3j+r) = A349876(5j+r+3)); computed by scanning k =
 *            0,1,2,... and keeping running maxima (search, memoized across
 *            calls)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

typedef unsigned __int128 A349896_u128;

static seq_t A349896_from_u128(A349896_u128 x)
{
    uint64_t hi = (uint64_t)(x >> 64);
    uint64_t lo = (uint64_t)x;
    seq_t r = (seq_t)hi;
    r *= (seq_t)4294967296u;
    r *= (seq_t)4294967296u;
    r += (seq_t)lo;
    return r;
}

/* Returns true and sets *out if the trajectory stays native (< 2^124);
   otherwise sets *big to the exact terminal value computed in seq_t. */
static bool A349896_traj(uint64_t k, A349896_u128 *out, seq_t *big)
{
    const A349896_u128 lim = ((A349896_u128)1) << 124;
    A349896_u128 m = k;
    while (m % 3 != 0) {
        if (m >= lim) {
            seq_t s = A349896_from_u128(m);
            uint64_t steps = 100000;
            while (s % 3 != 0) {
                if (--steps == 0) break;
                seq_t q = s / 3;
                seq_t r = s % 3;
                s = 5 * q + r + 3;
            }
            *big = s;
            return false;
        }
        A349896_u128 q = m / 3;
        A349896_u128 r = m % 3;
        m = 5 * q + r + 3;
    }
    *out = m;
    return true;
}

static seq_t *A349896_rec = 0;
static idx_t A349896_cnt = 0;
static idx_t A349896_cap = 0;
static uint64_t A349896_next = 0;
static A349896_u128 A349896_best128 = 0;
static seq_t A349896_bestbig;
static bool A349896_isbig = false;

static void A349896_push(seq_t v)
{
    if (A349896_cnt == A349896_cap) {
        idx_t nc = A349896_cap ? A349896_cap * 2 : 64;
        if (A349896_rec == 0)
            A349896_rec = mem_new(seq_t, nc);
        else
            A349896_rec = mem_resize(seq_t, A349896_rec, nc);
        A349896_cap = nc;
    }
    A349896_rec[A349896_cnt++] = v;
}

seq_t a(idx_t n)
{
    while (A349896_cnt < n) {
        uint64_t k = A349896_next++;
        A349896_u128 v = 0;
        seq_t vb = 0;
        bool native = A349896_traj(k, &v, &vb);
        if (k == 0) {
            A349896_best128 = v;
            A349896_push(A349896_from_u128(v));
            continue;
        }
        if (native) {
            if (!A349896_isbig && v > A349896_best128) {
                A349896_best128 = v;
                A349896_push(A349896_from_u128(v));
            }
        } else {
            if (!A349896_isbig) {
                A349896_isbig = true;
                A349896_bestbig = vb;
                A349896_push(vb);
            } else if (vb > A349896_bestbig) {
                A349896_bestbig = vb;
                A349896_push(vb);
            }
        }
    }
    return A349896_rec[n - 1];
}
