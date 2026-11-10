/*
 * A325569  Numbers n for which A325565(n) < A325568(n).
 * https://oeis.org/A325569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve search. For every pair d*q = m <= L, d is "valid" for m
 *            when A048720(A065621(d), q) = m. A325565(m) = number of valid d,
 *            and A325567(m) = largest valid proper divisor (d = 1 is always
 *            valid). A325568(m) = 1 + A325568(A325567(m)), with A325568(1) =
 *            0. a(n) = n-th m with A325565(m) < A325568(...
 * Range:     u8 n <= 1, u16 n <= 87, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Validated: standard, pass, 68 terms through n = 995 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A325569_terms = 0;
static size_t A325569_nterms = 0;
static uint64_t A325569_lim = 0;

static uint64_t A325569_rev(uint64_t d)
{
    uint64_t low = d & (~d + 1);
    return d ^ ((d - low) << 1);
}

static void A325569_build(uint64_t L)
{
    uint16_t *cnt = (uint16_t *)calloc((size_t)L + 1, sizeof(uint16_t));
    uint32_t *best = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    uint8_t *st = (uint8_t *)calloc((size_t)L + 1, 1);
    uint64_t *C = (uint64_t *)malloc(((size_t)L + 1) * sizeof(uint64_t));
    uint64_t d, q, m;
    size_t k = 0, cap = 1024;

    for (d = 1; d <= L; d++) {
        uint64_t rd = A325569_rev(d);
        uint64_t Q = L / d;
        C[0] = 0;
        for (q = 1; q <= Q; q++) {
            /* C[q] = carryless product rd (x) q */
            C[q] = (C[q >> 1] << 1) ^ ((q & 1) ? rd : 0);
            if (C[q] == d * q) {
                cnt[d * q]++;
                if (q > 1) best[d * q] = (uint32_t)d; /* d increasing: keeps largest */
            }
        }
    }
    free(C);

    if (A325569_terms) free(A325569_terms);
    A325569_terms = (uint64_t *)malloc(cap * sizeof(uint64_t));
    st[1] = 0;
    for (m = 2; m <= L; m++) {
        uint32_t b = best[m] ? best[m] : 1;
        st[m] = (uint8_t)(st[b] + 1);
        if ((unsigned)cnt[m] < (unsigned)st[m]) {
            if (k == cap) {
                cap *= 2;
                A325569_terms = (uint64_t *)realloc(A325569_terms, cap * sizeof(uint64_t));
            }
            A325569_terms[k++] = m;
        }
    }
    A325569_nterms = k;
    A325569_lim = L;
    free(cnt);
    free(best);
    free(st);
}

seq_t a(idx_t n)
{
    uint64_t L = A325569_lim ? A325569_lim : ((uint64_t)1 << 15);
    if (A325569_lim == 0) A325569_build(L);
    while ((idx_t)A325569_nterms < n) {
        L *= 2;
        A325569_build(L);
    }
    return (seq_t)A325569_terms[n - 1];
}
