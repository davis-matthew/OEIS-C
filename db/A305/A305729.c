/*
 * A305729  Numbers k such that A000010(k) = A023900(k) and
 *          A000010(A023900(k)) = A023900(A000010(k)).
 * https://oeis.org/A305729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   k is a term iff k and phi(k) are both squarefree with an even
 *            number of prime factors; a(n) is found by a sieve search
 *            (doubling limit), cached
 * Range:     u8 n <= 8, u16 n <= 659, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9505 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *terms = NULL;
static uint64_t term_cnt = 0;

static void build_terms(uint64_t L)
{
    uint32_t *spf = (uint32_t *)malloc((L + 1) * sizeof(uint32_t));
    uint32_t *ph = (uint32_t *)malloc((L + 1) * sizeof(uint32_t));
    unsigned char *sq = (unsigned char *)malloc(L + 1);
    uint64_t i, j;
    for (i = 0; i <= L; i++) spf[i] = 0;
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    /* sq: 0 = not squarefree, 1 = squarefree even omega, 2 = squarefree odd omega */
    sq[0] = 0; sq[1] = 1; ph[0] = 0; ph[1] = 1;
    for (i = 2; i <= L; i++) {
        uint64_t p = spf[i], r = i / p;
        if (r % p == 0) {
            sq[i] = 0;
            ph[i] = (uint32_t)(ph[r] * p);
        } else {
            ph[i] = (uint32_t)(ph[r] * (p - 1));
            sq[i] = (sq[r] == 0) ? 0 : (unsigned char)(3 - sq[r]);
        }
    }
    free(terms);
    terms = (uint64_t *)malloc((L / 4 + 16) * sizeof(uint64_t));
    term_cnt = 0;
    for (i = 1; i <= L; i++) {
        if (sq[i] == 1 && sq[ph[i]] == 1) {
            if (term_cnt < L / 4 + 16) terms[term_cnt++] = i;
        }
    }
    free(spf); free(ph); free(sq);
}

seq_t a(idx_t n)
{
    static uint64_t limit = 0;
    if (n < 1) return 0;
    while (term_cnt < (uint64_t)n) {
        limit = (limit == 0) ? (1u << 16) : limit * 2;
        build_terms(limit);
    }
    return (seq_t)terms[n - 1];
}
