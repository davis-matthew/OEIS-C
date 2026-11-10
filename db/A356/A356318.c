/*
 * A356318  Numbers k such that the least j >= k for which k and A276086(j)
 *          are coprime is a nontrivial multiple of k, where A276086 is the
 *          primorial base exp-function.
 * https://oeis.org/A356318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over k = 1, 2, 3, ...: let j = A356309(k), the least j >=
 *            k whose primorial-base digits are 0 at every position whose
 *            prime divides k (A276086(j) is then coprime to k). Keep k when j
 *            > k and k | j. j is found directly from the digits of k instead
 *            of by stepping j up one at a time
 * Range:     u8 n <= 10, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define PB_L 16

static const uint64_t PB_P[PB_L] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};

/* least j >= k with primorial-base digit i == 0 whenever prime(i+1) | k;
   returns 0 if it cannot be represented (never for practical k) */
static uint64_t pb_least_j(uint64_t k)
{
    uint64_t place[PB_L];
    int d[PB_L];
    bool con[PB_L];
    uint64_t v = k;
    int i;

    place[0] = 1;
    for (i = 1; i < PB_L; ++i) place[i] = place[i - 1] * PB_P[i - 1];

    for (i = 0; i < PB_L; ++i) {
        d[i] = (int)(v % PB_P[i]);
        v /= PB_P[i];
        con[i] = (k % PB_P[i]) == 0;
    }
    if (v != 0) return 0;

    int pos = -1;
    for (i = PB_L - 1; i >= 0; --i)
        if (con[i] && d[i] != 0) { pos = i; break; }
    if (pos < 0) return k;

    for (i = 0; i <= pos; ++i) d[i] = 0;
    int q = pos + 1;
    for (;;) {
        if (q >= PB_L) return 0;
        if (con[q]) { d[q] = 0; ++q; continue; }
        d[q]++;
        if ((uint64_t)d[q] < PB_P[q]) break;
        d[q] = 0;
        ++q;
    }

    uint64_t j = 0;
    for (i = 0; i < PB_L; ++i) j += (uint64_t)d[i] * place[i];
    return j;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t nextk = 1;

    while (cnt < (size_t)n) {
        uint64_t k = nextk++;
        uint64_t j = pb_least_j(k);
        if (j > k && j % k == 0) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 64;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
                if (!terms) abort();
            }
            terms[cnt++] = k;
        }
    }
    return (seq_t)terms[(size_t)n - 1];
}
