/*
 * A399716  Integers k equal to the sum over A066417(t) mod t, for some steps,
 *          starting with t = k and then using the result for the next
 *          calculation.
 * https://oeis.org/A399716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   search. For k = 1, 2, 3, ..., set t = k and s = 0, then repeat r
 *            = A066417(t) mod t, s += r, t = r until s >= k or r = 0. k is a
 *            term when s reaches exactly k. A066417(t) = sigma(2t-1) +
 *            sigma(2t+1) + sigma(t/2^v)*2^(v+1) - 6t - 2, where v = v2(t) and
 *            A066417(1) = 0. Each block of t values gets i...
 * Range:     u8 n <= 8, u16 n <= 39, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Validated: standard, pass, 60 terms through n = 93 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A399716_W ((uint64_t)1 << 17)

static uint32_t *A399716_F = NULL;      /* F[t] = A066417(t) mod t */
static uint64_t A399716_Fcap = 0;
static uint64_t A399716_Flim = 0;       /* F valid for 1..Flim */
static uint64_t *A399716_res = NULL;
static size_t A399716_cnt = 0, A399716_rcap = 0;
static uint64_t A399716_sig1[A399716_W];      /* sigma(n), n in [lo,hi] */
static uint64_t A399716_sig2[A399716_W + 1];  /* sigma(m), odd m in [2lo-1,2hi+1] */

static int A399716_test(uint64_t k)
{
    uint64_t t = k, sum = 0;
    for (;;) {
        uint64_t r = A399716_F[t];
        if (r == 0) return 0;
        sum += r;
        if (sum == k) return 1;
        if (sum > k) return 0;
        t = r;
    }
}

static void A399716_extend(void)
{
    uint64_t lo = A399716_Flim + 1, hi = lo + A399716_W - 1;
    uint64_t i, d;

    if (A399716_Fcap <= hi) {
        uint64_t nc = A399716_Fcap ? A399716_Fcap : ((uint64_t)1 << 20);
        while (nc <= hi) nc *= 2;
        A399716_F = (uint32_t *)realloc(A399716_F, (size_t)nc * sizeof(uint32_t));
        A399716_Fcap = nc;
    }

    /* sigma(n) for n in [lo, hi] */
    for (i = 0; i < A399716_W; ++i) A399716_sig1[i] = 0;
    for (d = 1; d * d <= hi; ++d) {
        uint64_t q = (lo + d - 1) / d;
        if (q < d) q = d;
        uint64_t m = q * d;
        while (m <= hi) {
            A399716_sig1[m - lo] += d + (q != d ? q : 0);
            ++q;
            m += d;
        }
    }

    /* sigma(m) for odd m in [A, B] */
    {
        uint64_t A = 2 * lo - 1, B = 2 * hi + 1;
        for (i = 0; i <= A399716_W; ++i) A399716_sig2[i] = 0;
        for (d = 1; d * d <= B; d += 2) {
            uint64_t q = (A + d - 1) / d;
            if (q < d) q = d;
            if ((q & 1) == 0) ++q;
            uint64_t m = q * d;
            while (m <= B) {
                A399716_sig2[(m - A) / 2] += d + (q != d ? q : 0);
                q += 2;
                m += 2 * d;
            }
        }
    }

    for (uint64_t t = lo; t <= hi; ++t) {
        if (t == 1) { A399716_F[t] = 0; continue; }
        uint64_t sa = A399716_sig2[t - lo];
        uint64_t sb = A399716_sig2[t - lo + 1];
        unsigned v = 0;
        uint64_t o = t;
        while ((o & 1) == 0) { o >>= 1; ++v; }
        uint64_t p = (uint64_t)1 << (v + 1);
        uint64_t so = A399716_sig1[t - lo] / (p - 1);
        uint64_t total = sa + sb + so * p - 6 * t - 2;
        A399716_F[t] = (uint32_t)(total % t);
    }
    A399716_Flim = hi;

    for (uint64_t k = lo; k <= hi; ++k) {
        if (A399716_test(k)) {
            if (A399716_cnt == A399716_rcap) {
                A399716_rcap = A399716_rcap ? A399716_rcap * 2 : 64;
                A399716_res = (uint64_t *)realloc(A399716_res, A399716_rcap * sizeof(uint64_t));
            }
            A399716_res[A399716_cnt++] = k;
        }
    }
}

seq_t a(idx_t n)
{
    while (A399716_cnt < (size_t)n)
        A399716_extend();
    return (seq_t)A399716_res[n - 1];
}
