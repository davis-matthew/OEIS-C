/*
 * A364919  a(0) = 1; a(n) is the smallest number m not already in the
 *          sequence such that rad(m) divides A019565(n).
 * https://oeis.org/A364919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = least m not already in the sequence with rad(m)
 *            | A019565(n); computed by search (Hamming-style enumeration of
 *            the numbers built from the primes of A019565(n))
 * Range:     u8 n <= 51, u16 n <= 1471, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 82 terms through n = 9851 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

static const uint32_t A364919_pr[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

static seq_t *A364919_res = 0;   /* terms in index order */
static seq_t *A364919_used = 0;  /* same values, sorted */
static size_t A364919_cnt = 0, A364919_cap = 0;

/* position of first element >= v in sorted used; sets found if equal */
static size_t A364919_lb(const seq_t v, bool *found)
{
    size_t lo = 0, hi = A364919_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (A364919_used[mid] < v) lo = mid + 1; else hi = mid;
    }
    *found = (lo < A364919_cnt && A364919_used[lo] == v);
    return lo;
}

static void A364919_push(seq_t v)
{
    if (A364919_cnt == A364919_cap) {
        size_t nc = A364919_cap ? A364919_cap * 2 : 1024;
        seq_t *nr = mem_new(seq_t, nc);
        seq_t *nu = mem_new(seq_t, nc);
        for (size_t i = 0; i < A364919_cnt; i++) {
            nr[i] = A364919_res[i];
            nu[i] = A364919_used[i];
        }
        if (A364919_res) mem_free(A364919_res);
        if (A364919_used) mem_free(A364919_used);
        A364919_res = nr;
        A364919_used = nu;
        A364919_cap = nc;
    }
    bool f;
    size_t pos = A364919_lb(v, &f);
    for (size_t i = A364919_cnt; i > pos; i--) A364919_used[i] = A364919_used[i - 1];
    A364919_used[pos] = v;
    A364919_res[A364919_cnt] = v;
    A364919_cnt++;
}

static seq_t A364919_term(idx_t t)
{
    if (t == 0) return (seq_t)1;
    uint32_t ps[64];
    size_t pc = 0;
    for (int i = 0; i < 64; i++)
        if ((t >> i) & 1ULL) ps[pc++] = A364919_pr[i];

    size_t hcap = 64, hlen = 1, pos = 0;
    seq_t *h = mem_new(seq_t, hcap);
    size_t ptr[64];
    h[0] = 1;
    for (size_t i = 0; i < pc; i++) ptr[i] = 0;

    seq_t answer = 1;
    for (;;) {
        if (pos == hlen) {
            seq_t best = h[ptr[0]] * (seq_t)ps[0];
            for (size_t i = 1; i < pc; i++) {
                seq_t c = h[ptr[i]] * (seq_t)ps[i];
                if (c < best) best = c;
            }
            for (size_t i = 0; i < pc; i++)
                if (h[ptr[i]] * (seq_t)ps[i] == best) ptr[i]++;
            if (hlen == hcap) {
                size_t nc = hcap * 2;
                seq_t *nh = mem_new(seq_t, nc);
                for (size_t i = 0; i < hlen; i++) nh[i] = h[i];
                mem_free(h);
                h = nh;
                hcap = nc;
            }
            h[hlen++] = best;
        }
        bool f;
        A364919_lb(h[pos], &f);
        if (!f) { answer = h[pos]; break; }
        pos++;
    }
    mem_free(h);
    return answer;
}

seq_t a(idx_t n)
{
    while ((idx_t)A364919_cnt <= n) {
        seq_t v = A364919_term((idx_t)A364919_cnt);
        A364919_push(v);
    }
    return A364919_res[n];
}
