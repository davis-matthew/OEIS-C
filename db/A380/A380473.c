/*
 * A380473  Numbers k neither squarefree nor prime power (i.e., in A126706)
 *          such that A119288(k) <= A003557(k) < A053669(k) < A006530(k).
 * https://oeis.org/A380473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k (increasing) that is neither squarefree nor a
 *            prime power, with A119288(k) <= A003557(k) < A053669(k) <
 *            A006530(k); found by incremental search with trial-division
 *            factoring, memoized
 * Range:     u8 n <= 4, u16 n <= 2151, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9770 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A380473_terms = 0;
static size_t A380473_cnt = 0, A380473_cap = 0;
static uint64_t A380473_next = 2;

static int A380473_test(uint64_t k)
{
    uint64_t m = k, pr[20], rad = 1;
    int ex[20], c = 0, nonsf = 0;
    for (uint64_t d = 2; d * d <= m; d++) {
        if (m % d == 0) {
            int e = 0;
            while (m % d == 0) { m /= d; e++; }
            pr[c] = d; ex[c] = e; c++;
            rad *= d;
            if (e > 1) nonsf = 1;
        }
    }
    if (m > 1) { pr[c] = m; ex[c] = 1; c++; rad *= m; }
    if (c < 2 || !nonsf) return 0;
    uint64_t p = pr[1], g = pr[c - 1], r = k / rad, q = 2;
    for (;;) {
        if (k % q != 0) break;
        q++;
        for (;;) {
            int isp = 1;
            for (uint64_t d = 2; d * d <= q; d++)
                if (q % d == 0) { isp = 0; break; }
            if (isp) break;
            q++;
        }
    }
    return p <= r && r < q && q < g;
}

seq_t a(idx_t n)
{
    while (A380473_cnt < (size_t)n) {
        if (A380473_test(A380473_next)) {
            if (A380473_cnt == A380473_cap) {
                A380473_cap = A380473_cap ? A380473_cap * 2 : 1024;
                A380473_terms = (uint64_t *)realloc(A380473_terms, A380473_cap * sizeof(uint64_t));
            }
            A380473_terms[A380473_cnt++] = A380473_next;
        }
        A380473_next++;
    }
    return (seq_t)A380473_terms[n - 1];
}
