/*
 * A352194  It seems that the n-th prime p appears in A064413 at around
 *          position 2*p; if in fact A064413(w) = p then set a(n) = 2*p-w.
 * https://oeis.org/A352194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*prime(n) - A064955(n), where A064955(n) is the position
 *            of the n-th prime in the EKG sequence A064413; computed by
 *            simulating the EKG sequence
 * Range:     i8 n <= 232, i16 n <= 690, i32 n <= 690, i64 n <= 690,
 *            i128 n <= 690, big any n
 * Validated: standard, pass, 104 terms through n = 664 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

static long long *a352194_val = NULL;
static size_t a352194_cnt = 0;

static void a352194_run(size_t target)
{
    double t = (double)target;
    size_t pb = (target < 6) ? 20 : (size_t)(t * (log(t) + log(log(t)))) + 10;
    unsigned char *comp = (unsigned char *)calloc(pb + 1, 1);
    size_t i, j, k = 0, p = 2, L, found = 0, step;
    uint32_t *spf, *nxt, prev, cur;
    unsigned char *used;
    long long *res;

    if (!comp) return;
    for (i = 2; i * i <= pb; i++)
        if (!comp[i])
            for (j = i * i; j <= pb; j += i) comp[j] = 1;
    for (i = 2; i <= pb; i++)
        if (!comp[i]) {
            if (++k == target) { p = i; break; }
        }
    free(comp);
    L = 4 * p + 100;

    spf = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    nxt = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    used = (unsigned char *)calloc(L + 1, 1);
    res = (long long *)malloc((target + 1) * sizeof(long long));
    if (!spf || !nxt || !used || !res) {
        free(spf); free(nxt); free(used); free(res);
        return;
    }
    for (i = 2; i <= L; i++)
        if (spf[i] == 0)
            for (j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;

    used[1] = 1; used[2] = 1;
    res[found++] = 2LL * 2 - 2;
    prev = 2;
    step = 2;
    while (found < target) {
        uint32_t x = prev, best = 0xFFFFFFFFu;
        while (x > 1) {
            uint32_t q = spf[x];
            uint32_t c = nxt[q] ? nxt[q] : q;
            while (c <= L && used[c]) c += q;
            nxt[q] = c;
            if (c < best) best = c;
            while (x % q == 0) x /= q;
        }
        if (best > L) break;
        cur = best;
        used[cur] = 1;
        step++;
        prev = cur;
        if (spf[cur] == cur) res[found++] = 2LL * (long long)cur - (long long)step;
    }
    free(a352194_val);
    a352194_val = res;
    a352194_cnt = found;
    free(spf); free(nxt); free(used);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if ((size_t)n > a352194_cnt) {
        size_t target = (size_t)n;
        if (target < 2 * a352194_cnt) target = 2 * a352194_cnt;
        if (target < 100) target = 100;
        a352194_run(target);
        if ((size_t)n > a352194_cnt) return (seq_t)0;
    }
    return (seq_t)a352194_val[n - 1];
}
