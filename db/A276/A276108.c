/*
 * A276108  Numbers expressible as perfect powers in a composite number of
 *          ways.
 * https://oeis.org/A276108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest of {1} U {m^k : m>=2 not a perfect power,
 *            k>=2 with d(k)-1 composite (d = number of divisors)}; computed
 *            by a merge of the k-th power streams
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 15, u128 n <= 253,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 9664 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

#define A276108_MAXK 512

static bool A276108_is_pp(uint64_t m)
{
    if (m < 4) return false;
    for (uint64_t b = 2; b * b <= m; b++) {
        uint64_t p = b * b;
        while (1) {
            if (p == m) return true;
            if (p > m / b) break;
            p *= b;
        }
    }
    return false;
}

static uint64_t A276108_next_np(uint64_t m)
{
    m++;
    while (A276108_is_pp(m)) m++;
    return m;
}

static bool A276108_valid_k(uint64_t k)
{
    if (k < 2) return false;
    uint64_t d = 0;
    for (uint64_t i = 1; i * i <= k; i++) {
        if (k % i == 0) d += (i * i == k) ? 1 : 2;
    }
    uint64_t w = d - 1;
    if (w < 4) return false;
    for (uint64_t i = 2; i * i <= w; i++)
        if (w % i == 0) return true;
    return false;
}

static bool A276108_pow_sat(uint64_t m, uint64_t k, seq_t *out)
{
    seq_t r = 1;
    seq_t mm = (seq_t)m;
    for (uint64_t i = 0; i < k; i++) {
        seq_t t = r * mm;
        if (t / mm != r) return false;
        r = t;
    }
    *out = r;
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t len = 0, cap = 0;
    static uint64_t ks[A276108_MAXK];
    static uint64_t ms[A276108_MAXK];
    static bool inf[A276108_MAXK];
    static seq_t *vals = NULL;
    static int nact = 0;
    static uint64_t pending = 1;
    static bool finished = false;

    if (!vals) vals = mem_new(seq_t, A276108_MAXK);
    if (len == 0) {
        cap = 64;
        terms = mem_new(seq_t, cap);
        terms[len++] = 1;
        pending = 1;
        do { pending++; } while (!A276108_valid_k(pending));
    }

    while (len < (size_t)n && !finished) {
        int best = -1;
        for (;;) {
            best = -1;
            for (int i = 0; i < nact; i++) {
                if (inf[i]) continue;
                if (best < 0 || vals[i] < vals[best]) best = i;
            }
            seq_t p;
            if (nact < A276108_MAXK && A276108_pow_sat(2, pending, &p) &&
                (best < 0 || p <= vals[best])) {
                ks[nact] = pending;
                ms[nact] = 2;
                vals[nact] = p;
                inf[nact] = false;
                nact++;
                do { pending++; } while (!A276108_valid_k(pending));
                continue;
            }
            break;
        }
        if (best < 0) { finished = true; break; }

        if (len >= cap) {
            size_t nc = cap * 2;
            seq_t *tmp = mem_resize(seq_t, terms, nc);
            if (!tmp) { finished = true; break; }
            terms = tmp;
            cap = nc;
        }
        terms[len++] = vals[best];

        ms[best] = A276108_next_np(ms[best]);
        seq_t v;
        if (A276108_pow_sat(ms[best], ks[best], &v)) vals[best] = v;
        else inf[best] = true;
    }

    if ((size_t)n >= 1 && (size_t)n <= len) return terms[n - 1];
    return (seq_t)0;
}
