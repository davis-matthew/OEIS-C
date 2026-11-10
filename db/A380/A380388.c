/*
 * A380388  Primes q for which A177436(pi(q)) is not equal to 2*(1 +
 *          floor(log_2(q/(2^ceiling(log_2(q))-q)))).
 * https://oeis.org/A380388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th prime q (q = 3, 5 first) with A177436(pi(q)) !=
 *            2*(1+floor(log_2(q/(2^ceiling(log_2(q))-q)))). Search: for each
 *            bit length K, d = 2^K - q; the two expressions can differ only
 *            when 2^j*d lies in [q-4, q], i.e. (2^j+1)*d = 2^K - r with r =
 *            1..4, or when q = 2^(K-1)+1 or 2^(K-1)+3 (speci...
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-10)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

static seq_t *A380388_res = 0;
static size_t A380388_cnt = 0;
static size_t A380388_cap = 0;
static unsigned A380388_K = 4;
static seq_t A380388_t = 15;
static bool A380388_done = false;
static bool A380388_init = false;

static void A380388_push(seq_t v)
{
    if (A380388_cnt == A380388_cap) {
        size_t nc = A380388_cap ? A380388_cap * 2 : 256;
        seq_t *nr = mem_new(seq_t, nc);
        for (size_t i = 0; i < A380388_cnt; i++) nr[i] = A380388_res[i];
        if (A380388_res) mem_free(A380388_res);
        A380388_res = nr;
        A380388_cap = nc;
    }
    A380388_res[A380388_cnt++] = v;
}

static unsigned A380388_flog2(seq_t x)
{
    unsigned c = 0;
    while (x > 1) { x >>= 1; c++; }
    return c;
}

static bool A380388_ispow2(seq_t v)
{
    return v > 0 && ((v & (v - 1)) == 0);
}

static void A380388_process(void)
{
    unsigned K = A380388_K;
    seq_t t = A380388_t;          /* 2^K - 1 */
    seq_t half = (t >> 1) + 1;    /* 2^(K-1) */
    size_t cap = (size_t)4 * K + 8;
    seq_t *cand = mem_new(seq_t, cap);
    size_t cc = 0;
    cand[cc++] = half + 1;
    cand[cc++] = half + 3;
    for (int r = 1; r <= 4; r++) {
        seq_t nr = t - (seq_t)(r - 1);
        for (unsigned j = 1; j < K; j++) {
            seq_t m = ((seq_t)1 << j) + 1;
            if (m > nr) break;
            if (nr % m == 0) {
                seq_t d = nr / m;
                if (cc < cap) cand[cc++] = t - (d - 1);
            }
        }
    }
    /* insertion sort */
    for (size_t i = 1; i < cc; i++) {
        seq_t v = cand[i];
        size_t j = i;
        while (j > 0 && cand[j - 1] > v) { cand[j] = cand[j - 1]; j--; }
        cand[j] = v;
    }
    bool have_last = false;
    seq_t last = 0;
    for (size_t i = 0; i < cc; i++) {
        seq_t p = cand[i];
        if (have_last && p == last) continue;
        have_last = true; last = p;
        if (!(p > half) || p > t) continue;
        if (!is_prime_seq(p)) continue;
        seq_t d = t - p + 1;
        seq_t F = (seq_t)2 * ((seq_t)1 + (seq_t)A380388_flog2(p / d));
        seq_t A = 0;
        bool set = false;
        if (t % 5 == 2) {
            seq_t pp = (t / 5) * 4 + 3;
            if (p == pp) {
                unsigned e = K + 2;
                if (e >= 9 && ((e - 1) & 3) == 0) { A = 5; set = true; }
            }
        }
        if (!set && p == half + 1) {
            unsigned ee = K - 1;
            if (ee >= 4 && A380388_ispow2((seq_t)ee)) { A = 4; set = true; }
        }
        if (!set && p == half + 3 && K - 1 >= 3) { A = 3; set = true; }
        if (!set) {
            A = (seq_t)2 * ((seq_t)1 + (seq_t)A380388_flog2((p - 5) / d));
        }
        if (A != F) A380388_push(p);
    }
    mem_free(cand);

    seq_t tn = (t << 1) | 1;
    if (!(tn > t) || K >= 4000) {
        A380388_done = true;
    } else {
        A380388_t = tn;
        A380388_K = K + 1;
    }
}

seq_t a(idx_t n)
{
    if (!A380388_init) {
        A380388_init = true;
        A380388_push(3);
        A380388_push(5);
    }
    while (A380388_cnt < (size_t)n && !A380388_done)
        A380388_process();
    if (n < 1 || (size_t)n > A380388_cnt) return (seq_t)0;
    return A380388_res[(size_t)n - 1];
}
