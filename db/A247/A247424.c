/*
 * A247424  Odd numbers not of the form 2*A005206(A003249(m)) - 1.
 * https://oeis.org/A247424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*k - 1 where k is the n-th positive integer not in
 *            {A005206(A003249(m)) : m >= 1}; computed by a merge scan
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-07)
 * Helpers:   isqrt_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/isqrt_u64.h"

/* ---- A005206 ---- */
static seq_t *G_memo = NULL;
static idx_t G_memo_size = 0;
static idx_t G_memo_computed = 0;

static seq_t A005206_a(idx_t n) {
    if (n < G_memo_computed) {
        return G_memo[n];
    }
    if (G_memo_size == 0) {
        G_memo_size = 1000;
        G_memo = mem_new(seq_t, G_memo_size);
    }
    while (n >= G_memo_size) {
        idx_t new_size = G_memo_size * 2;
        seq_t *new_memo = mem_new(seq_t, new_size);
        for (idx_t i = 0; i < G_memo_computed; i++) {
            new_memo[i] = G_memo[i];
        }
        mem_free(G_memo);
        G_memo = new_memo;
        G_memo_size = new_size;
    }
    for (idx_t i = G_memo_computed; i <= n; i++) {
        if (i == 0) {
            G_memo[i] = 0;
        } else {
            seq_t prev_val = G_memo[i - 1];
            idx_t prev_idx = (idx_t)prev_val;
            G_memo[i] = (seq_t)i - G_memo[prev_idx];
        }
    }
    G_memo_computed = n + 1;
    return G_memo[n];
}

/* ---- A003249 ---- */
static uint64_t w_L(uint64_t m) { return (m + isqrt_u64(5 * m * m)) / 2; }
static uint64_t w_B(uint64_t m) { return w_L(m) + 2 * m; }
static uint64_t w_U(uint64_t m) { return w_L(m) + m; }

static uint64_t *w_list = NULL;
static idx_t w_cnt = 0, w_cap = 0;
static uint64_t w_next = 1;

static seq_t A003249_a(idx_t n) {
    while (w_cnt < n) {
        uint64_t m = w_next++;
        if (w_U(w_B(m)) == w_B(w_U(m)) + 1) {
            if (w_cnt == w_cap) {
                idx_t nc = w_cap ? 2 * w_cap : 1024;
                uint64_t *p = (uint64_t *)realloc(w_list, (size_t)nc * sizeof(uint64_t));
                if (!p) return (seq_t)0;
                w_list = p;
                w_cap = nc;
            }
            w_list[w_cnt++] = m;
        }
    }
    {
        uint64_t c = w_list[n - 1];
        seq_t r = (seq_t)w_U(c);
        r += 1;
        return r;
    }
}

static seq_t T_val(idx_t m) {
    seq_t c = A003249_a(m);
    return A005206_a((idx_t)c);
}

seq_t a(idx_t n) {
    idx_t m = 1;
    seq_t tm = T_val(m);
    idx_t cnt = 0;
    idx_t k = 0;
    for (;;) {
        k++;
        while (tm < (seq_t)k) {
            m++;
            tm = T_val(m);
        }
        if (tm != (seq_t)k) {
            cnt++;
            if (cnt == n) {
                seq_t r = (seq_t)k;
                return 2 * r - 1;
            }
        }
    }
}
