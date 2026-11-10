/*
 * A370513  Complement of A323599.
 * https://oeis.org/A370513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer not of the form A323599(k), where
 *            A323599(k) = Sum_{p|k, p prime} sigma(k/p); computed by a sieve
 *            over k
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A370513_val(uint64_t m)
{
    if (m == 1) return 0;
    uint64_t pr[64], ex[64];
    size_t c = 0;
    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) { t /= p; e++; }
            pr[c] = p; ex[c] = e; c++;
        }
    }
    if (t > 1) { pr[c] = t; ex[c] = 1; c++; }
    uint64_t res = 0;
    for (size_t i = 0; i < c; i++) {
        uint64_t s = 1;
        for (size_t j = 0; j < c; j++) {
            uint64_t e = ex[j] - (j == i ? 1 : 0);
            uint64_t term = 1, pw = 1;
            for (uint64_t k = 0; k < e; k++) { pw *= pr[j]; term += pw; }
            s *= term;
        }
        res += s;
    }
    return res;
}

static uint64_t *A370513_list = 0;
static size_t A370513_cnt = 0;

static void A370513_build(uint64_t V)
{
    unsigned char *mark = (unsigned char *)calloc((size_t)V + 2, 1);
    unsigned char *comp = (unsigned char *)calloc((size_t)V + 2, 1);
    uint64_t *pl = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)V + 2));
    size_t pc = 0;
    for (uint64_t i = 2; i <= V; i++) {
        if (!comp[i]) {
            pl[pc++] = i;
            for (uint64_t j = i * i; j <= V; j += i) comp[j] = 1;
        }
    }
    mark[0] = 1; mark[1] = 1;
    for (size_t i = 0; i < pc; i++) {
        if (pl[i] + 1 <= V) mark[pl[i] + 1] = 1;
    }
    for (size_t i = 0; i < pc; i++) {
        for (size_t j = i + 1; j < pc; j++) {
            uint64_t s = pl[i] + pl[j] + 2;
            if (s > V) break;
            mark[s] = 1;
        }
    }
    uint64_t V3 = V * V * V;
    uint64_t L = 1;
    while (L * L < V3) L++;
    for (uint64_t m = 2; m <= L; m++) {
        uint64_t v = A370513_val(m);
        if (v <= V) mark[v] = 1;
    }
    free(A370513_list);
    A370513_list = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)V + 2));
    A370513_cnt = 0;
    for (uint64_t i = 1; i <= V; i++)
        if (!mark[i]) A370513_list[A370513_cnt++] = i;
    free(mark); free(comp); free(pl);
}

seq_t a(idx_t n)
{
    static uint64_t curV = 0;
    if (n < 1) return 0;
    while (A370513_cnt < (size_t)n || curV == 0) {
        curV = curV ? curV * 2 : 64;
        A370513_build(curV);
    }
    return (seq_t)A370513_list[n - 1];
}
