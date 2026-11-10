/*
 * A348199  a(n) is the least term of A326835 whose number of divisors is n.
 * https://oeis.org/A348199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. a(n) is the smallest odd m with
 *            tau(m) = n whose divisors all have distinct phi values. It is
 *            found by a depth-first search over m = p1^(d1-1) * p2^(d2-1) *
 *            ... with primes p1 < p2 < ..., d1*d2*... = n, and the search is
 *            cut off by the bound a(n) <= 3^(n-1)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t a348199_best;
static uint64_t a348199_P[70];
static int a348199_E[70];
static seq_t *a348199_tmp;

static bool a348199_isprime(uint64_t p)
{
    if (p < 2) return false;
    if (p % 2 == 0) return p == 2;
    for (uint64_t d = 3; d * d <= p; d += 2)
        if (p % d == 0) return false;
    return true;
}

/* true if the divisors of prod P[i]^E[i] (i<depth) have distinct phi values */
static bool a348199_check(int depth)
{
    size_t cnt = 1;
    a348199_tmp[0] = 1;
    for (int i = 0; i < depth; i++) {
        size_t base = cnt;
        seq_t pm = 1;
        seq_t pm1 = (seq_t)(a348199_P[i] - 1);
        for (int j = 1; j <= a348199_E[i]; j++) {
            seq_t ph = pm * pm1;
            for (size_t k = 0; k < base; k++)
                a348199_tmp[cnt++] = a348199_tmp[k] * ph;
            pm *= (seq_t)a348199_P[i];
        }
    }
    for (size_t i = 1; i < cnt; i++)
        for (size_t j = 0; j < i; j++)
            if (a348199_tmp[i] == a348199_tmp[j]) return false;
    return true;
}

/* minimal total exponent needed to realise r divisors: sum over prime
   factors q of r (with multiplicity) of (q - 1) */
static int a348199_minexp(idx_t r)
{
    int s = 0;
    idx_t t = r;
    for (idx_t q = 2; q * q <= t; q++)
        while (t % q == 0) { t /= q; s += (int)(q - 1); }
    if (t > 1) s += (int)(t - 1);
    return s;
}

static void a348199_dfs(int depth, uint64_t prevp, seq_t cur, idx_t r)
{
    int me = a348199_minexp(r);
    for (uint64_t p = prevp + 2; ; p += 2) {
        if (!a348199_isprime(p)) continue;
        seq_t limit = (a348199_best - 1) / cur;
        if (limit < (seq_t)p) break;
        {
            /* all remaining primes are >= p, so the remainder is >= p^me */
            seq_t pw = 1;
            bool over = false;
            for (int i = 0; i < me; i++) {
                if (pw > limit / (seq_t)p) { over = true; break; }
                pw *= (seq_t)p;
            }
            if (over) break;
        }
        for (idx_t d = 2; d <= r; d++) {
            if (r % d != 0) continue;
            int e = (int)(d - 1);
            limit = (a348199_best - 1) / cur;
            seq_t pw = 1;
            bool over = false;
            for (int i = 0; i < e; i++) {
                if (pw > limit / (seq_t)p) { over = true; break; }
                pw *= (seq_t)p;
            }
            if (over) break;
            a348199_P[depth] = p;
            a348199_E[depth] = e;
            if (!a348199_check(depth + 1)) break;
            seq_t nc = cur * pw;
            if (r / d == 1) {
                if (nc < a348199_best) a348199_best = nc;
            } else {
                a348199_dfs(depth + 1, p, nc, r / d);
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return 1;
    seq_t b = 1;
    bool overflow = false;
    for (idx_t i = 1; i < n; i++) {
        seq_t nb = b * 3;
        if (nb / 3 != b) { overflow = true; break; }
        b = nb;
    }
    if (overflow) {
        b = (seq_t)0;
        b -= 1;              /* saturate (only reachable for fixed-width seq_t) */
        a348199_best = b;
    } else {
        a348199_best = b + 1; /* 3^(n-1) itself is a valid candidate */
    }
    a348199_tmp = mem_new(seq_t, (size_t)n + 1);
    a348199_dfs(0, 1, 1, n);
    mem_free(a348199_tmp);
    return a348199_best;
}
