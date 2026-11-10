/*
 * A334090  a(1) = 0, and then after the first differences of A064097.
 * https://oeis.org/A334090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 0, a(n) = A064097(n) - A064097(n-1) for n > 1, where
 *            A064097(1) = 0, A064097(p) = 1 + A064097(p-1) for prime p,
 *            A064097(m*k) = A064097(m) + A064097(k); computed with a
 *            smallest-prime-factor sieve
 * Range:     i8 n <= 65537, i16 n <= 65537, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 6942 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *q_val = NULL;
static idx_t *q_spf = NULL;
static idx_t q_done = 0;

static void q_ensure(idx_t n)
{
    if (n <= q_done) return;
    idx_t sz = n;
    if (sz < 2 * q_done) sz = 2 * q_done;
    if (sz < 16) sz = 16;
    if (q_val) mem_free(q_val);
    if (q_spf) free(q_spf);
    q_val = mem_new(seq_t, sz + 1);
    q_spf = (idx_t *)calloc((size_t)sz + 1, sizeof(idx_t));
    for (idx_t i = 2; i <= sz; ++i) {
        if (q_spf[i] == 0) {
            q_spf[i] = i;
            if (i <= sz / i)
                for (idx_t j = i * i; j <= sz; j += i)
                    if (q_spf[j] == 0) q_spf[j] = i;
        }
    }
    q_val[1] = 0;
    for (idx_t i = 2; i <= sz; ++i) {
        if (q_spf[i] == i) q_val[i] = q_val[i - 1] + 1;
        else q_val[i] = q_val[q_spf[i]] + q_val[i / q_spf[i]];
    }
    q_done = sz;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    q_ensure(n);
    return q_val[n] - q_val[n - 1];
}
