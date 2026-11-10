/*
 * A378658  a(n) = A337345(A091191(n)), where A337345 is the number of
 *          divisors d of n for which A003961(d) > 2*d, and A091191 lists the
 *          primitive abundant numbers.
 * https://oeis.org/A378658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A337345(A091191(n)), i.e. a(n) is the number of divisors
 *            d of m = A091191(n) (the n-th primitive abundant number) with
 *            A003961(d) > 2*d. A sieve finds the primitive abundant numbers:
 *            abundant numbers with no abundant proper divisor (a perfect
 *            proper divisor is allowed). The code then count...
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 120 terms through n = 9670 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *pa_list = NULL;
static size_t pa_cnt = 0;
static uint64_t pa_limit = 0;

static void pa_build(uint64_t L)
{
    uint64_t *sig = (uint64_t *)calloc((size_t)(L + 1), sizeof(uint64_t));
    unsigned char *flag = (unsigned char *)calloc((size_t)(L + 1), 1);
    size_t cap = 1024, c = 0;
    uint64_t *lst = (uint64_t *)malloc(cap * sizeof(uint64_t));
    uint64_t i, j;
    for (i = 1; i <= L; i++)
        for (j = i; j <= L; j += i)
            sig[j] += i;
    for (i = 2; i <= L; i++) {
        if (sig[i] > 2 * i) {
            if (!flag[i]) {
                if (c == cap) {
                    cap *= 2;
                    lst = (uint64_t *)realloc(lst, cap * sizeof(uint64_t));
                }
                lst[c++] = i;
            }
            for (j = 2 * i; j <= L; j += i)
                flag[j] = 1;
        }
    }
    free(sig);
    free(flag);
    free(pa_list);
    pa_list = lst;
    pa_cnt = c;
    pa_limit = L;
}

static uint64_t pa_nextprime(uint64_t p)
{
    uint64_t q = p + 1;
    for (;;) {
        bool ok = true;
        uint64_t k;
        for (k = 2; k * k <= q; k++)
            if (q % k == 0) { ok = false; break; }
        if (ok) return q;
        q++;
    }
}

static void pa_enum(const uint64_t *pr, const uint64_t *ex, size_t len,
                    size_t pos, uint64_t d, uint64_t av, const uint64_t *np,
                    uint64_t *cnt)
{
    uint64_t e;
    if (pos == len) {
        if (av > 2 * d) ++*cnt;
        return;
    }
    for (e = 0; e <= ex[pos]; e++) {
        pa_enum(pr, ex, len, pos + 1, d, av, np, cnt);
        d *= pr[pos];
        av *= np[pos];
    }
}

seq_t a(idx_t n)
{
    uint64_t m, t, p;
    uint64_t pr[24], ex[24], np[24];
    size_t len = 0, k;
    uint64_t cnt = 0;

    if (pa_limit == 0) pa_build((uint64_t)1 << 16);
    while (pa_cnt < (size_t)n)
        pa_build(pa_limit * 2);
    m = pa_list[(size_t)(n - 1)];

    t = m;
    for (p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            pr[len] = p; ex[len] = 0;
            while (t % p == 0) { t /= p; ex[len]++; }
            len++;
        }
    }
    if (t > 1) { pr[len] = t; ex[len] = 1; len++; }
    for (k = 0; k < len; k++) np[k] = pa_nextprime(pr[k]);

    pa_enum(pr, ex, len, 0, 1, 1, np, &cnt);
    return (seq_t)cnt;
}
