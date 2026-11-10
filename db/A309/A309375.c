/*
 * A309375  Unlucky palindromic primes.
 * https://oeis.org/A309375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search. Go through the palindromic primes (A002385) in
 *            increasing order and keep the ones that are not lucky numbers
 *            (A000959), i.e. that are in A050505. To test whether a number is
 *            lucky, follow its position backwards through the lucky sieve.
 *            Long stretches with the same floor(p/s) are handled i...
 * Range:     u8 n <= 7, u16 n <= 61, u32 n <= 5527, u64 n <= 5527,
 *            u128 n <= 5527, big any n
 * Validated: standard, pass, 58 terms through n = 5355 (2026-11-08)
 * Helpers:   is_prime_u64, mulmod, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/powmod_u64.h"

static uint64_t *A309375_cache = NULL;
static size_t A309375_cnt = 0, A309375_cap = 0;

/* palindromic prime generator state */
static int A309375_small_i = 0;
static int A309375_k = 1;
static uint64_t A309375_pk = 10;
static uint64_t A309375_h = 10;

/* lucky numbers: key[j] = lucky[j] + j (strictly increasing), all luckies <= LK_L */
static uint32_t *A309375_key = NULL;
static size_t A309375_klen = 0;
static uint64_t A309375_L = 0;

static uint64_t A309375_next_pp(void) {
    static const uint64_t small[5] = {2, 3, 5, 7, 11};
    if (A309375_small_i < 5) return small[A309375_small_i++];
    while (A309375_k <= 9) {
        uint64_t lead, p, t;
        if (A309375_h >= A309375_pk * 10) {
            A309375_k++;
            A309375_pk *= 10;
            A309375_h = A309375_pk;
            continue;
        }
        lead = A309375_h / A309375_pk;
        if ((lead & 1) == 0 || lead == 5) {
            A309375_h = (lead + 1) * A309375_pk;
            continue;
        }
        p = A309375_h;
        t = A309375_h / 10;
        while (t) { p = p * 10 + t % 10; t /= 10; }
        A309375_h++;
        if (is_prime_u64(p)) return p;
    }
    return 0;
}

static size_t A309375_kth(const int32_t *tree, size_t n1, size_t logb, uint64_t k) {
    size_t pos = 0, b;
    for (b = logb; b; b >>= 1) {
        if (pos + b <= n1 && (uint64_t)tree[pos + b] < k) {
            pos += b;
            k -= (uint64_t)tree[pos];
        }
    }
    return pos + 1;
}

static void A309375_build(uint64_t M) {
    uint32_t t[128], f[64];
    uint32_t fc[64];
    int tsz = 0, nf = 0, i;
    size_t idx, sz, n1, cap, logb, j, r;
    uint32_t *arr;
    int32_t *tree;
    unsigned char *alive;
    uint64_t v, cur;

    /* small lucky numbers (< 256) by naive sieve */
    for (v = 1; v < 256; v += 2) t[tsz++] = (uint32_t)v;
    {
        int id = 1;
        while (id < tsz) {
            int step = (int)t[id], w = 0, q;
            if (step > tsz) break;
            for (q = 0; q < tsz; q++)
                if ((q + 1) % step != 0) t[w++] = t[q];
            tsz = w;
            id++;
        }
    }
    for (i = 1; i < tsz && t[i] < 64; i++) { f[nf] = t[i]; fc[nf] = 0; nf++; }

    /* stream odd numbers through the small-step passes */
    {
        uint64_t n0 = (M + 1) / 2;
        cap = (size_t)(n0 - n0 / 3 + 64);
    }
    arr = (uint32_t *)malloc(cap * sizeof(uint32_t));
    sz = 0;
    for (v = 1; v <= M; v += 2) {
        for (i = 0; i < nf; i++) {
            if (++fc[i] == f[i]) { fc[i] = 0; break; }
        }
        if (i == nf) arr[sz++] = (uint32_t)v;
    }

    /* remaining passes with a Fenwick tree */
    n1 = sz;
    tree = (int32_t *)malloc((n1 + 1) * sizeof(int32_t));
    alive = (unsigned char *)malloc(n1 + 1);
    tree[0] = 0;
    for (j = 1; j <= n1; j++) tree[j] = (int32_t)(j & (~j + 1));
    for (j = 0; j < n1; j++) alive[j] = 1;
    logb = 1;
    while (logb * 2 <= n1) logb *= 2;
    cur = n1;
    idx = (size_t)nf + 1;
    for (;;) {
        uint64_t step, k;
        size_t pos;
        if ((uint64_t)idx + 1 > cur) break;
        pos = A309375_kth(tree, n1, logb, (uint64_t)idx + 1);
        step = arr[pos - 1];
        if (step > cur) break;
        for (k = cur / step; k >= 1; k--) {
            pos = A309375_kth(tree, n1, logb, k * step);
            alive[pos - 1] = 0;
            for (r = pos; r <= n1; r += r & (~r + 1)) tree[r]--;
        }
        cur -= cur / step;
        idx++;
    }

    if (A309375_key) free(A309375_key);
    A309375_key = (uint32_t *)malloc(((size_t)cur + 1) * sizeof(uint32_t));
    j = 0;
    for (r = 0; r < n1; r++) {
        if (alive[r]) { A309375_key[j] = (uint32_t)(arr[r] + j); j++; }
    }
    A309375_klen = j;
    A309375_L = M;
    free(arr);
    free(tree);
    free(alive);
}

static bool A309375_is_lucky(uint64_t x) {
    uint64_t p, M, pw = 1;
    size_t j;
    int d = 0;
    if ((x & 1) == 0) return false;
    { uint64_t y = x; while (y) { d++; y /= 10; pw *= 10; } }
    M = pw / (uint64_t)(2 * d);
    if (M < 65536) M = 65536;
    if (M > ((uint64_t)1 << 31)) M = (uint64_t)1 << 31;
    if (A309375_L < M) A309375_build(M);

    p = (x + 1) / 2;
    j = 1;
    for (;;) {
        uint64_t s, q, T;
        size_t lo, hi;
        if (j >= A309375_klen) {
            if (p <= A309375_L) return true;
            A309375_build(A309375_L * 2);
            continue;
        }
        s = (uint64_t)A309375_key[j] - j;
        if (s > p) return true;
        if (p % s == 0) return false;
        q = p / s;
        T = p / q + j;
        lo = j; hi = A309375_klen - 1;
        while (lo < hi) {
            size_t mid = lo + (hi - lo + 1) / 2;
            if ((uint64_t)A309375_key[mid] <= T) lo = mid; else hi = mid - 1;
        }
        if (p % q == 0 && (uint64_t)A309375_key[lo] == T) return false;
        p -= q * (uint64_t)(lo - j + 1);
        j = lo + 1;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    while (A309375_cnt < n) {
        uint64_t v = A309375_next_pp();
        if (v == 0) return (seq_t)0;
        if (!A309375_is_lucky(v)) {
            if (A309375_cnt == A309375_cap) {
                size_t nc = A309375_cap ? A309375_cap * 2 : 1024;
                A309375_cache = (uint64_t *)realloc(A309375_cache, nc * sizeof(uint64_t));
                A309375_cap = nc;
            }
            A309375_cache[A309375_cnt++] = v;
        }
    }
    return (seq_t)A309375_cache[n - 1];
}
