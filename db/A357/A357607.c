/*
 * A357607  Odd numbers k such that A162296(k) > 2*k.
 * https://oeis.org/A357607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd k with A162296(k) > 2*k, where A162296(k) =
 *            sigma(k) - prod_{p|k}(1+p); found by a segmented sieve over odd
 *            numbers, cached across calls
 * Range:     u8 none, u16 n <= 41, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 54 terms through n = 9761 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A357607_terms = 0;
static size_t A357607_cnt = 0, A357607_cap = 0;
static uint64_t A357607_next = 1; /* next odd number to examine */

static void A357607_block(void)
{
    const size_t B = 65536;
    uint64_t lo = A357607_next;
    uint64_t hi = lo + 2 * (uint64_t)B; /* exclusive */
    uint64_t lim = 1;
    while (lim * lim < hi) lim++;
    char *comp = (char *)calloc((size_t)lim + 2, 1);
    uint64_t *rem = (uint64_t *)malloc(B * sizeof(uint64_t));
    uint64_t *sig = (uint64_t *)malloc(B * sizeof(uint64_t));
    uint64_t *sf = (uint64_t *)malloc(B * sizeof(uint64_t));
    size_t i;
    uint64_t p;
    for (i = 0; i < B; i++) {
        rem[i] = lo + 2 * (uint64_t)i;
        sig[i] = 1;
        sf[i] = 1;
    }
    for (p = 3; p <= lim; p += 2) {
        if (comp[p]) continue;
        for (uint64_t q = p * p; q <= lim; q += p) comp[q] = 1;
        uint64_t m = ((lo + p - 1) / p) * p;
        if ((m & 1) == 0) m += p;
        for (; m < hi; m += 2 * p) {
            size_t idx = (size_t)((m - lo) / 2);
            uint64_t t = rem[idx];
            uint64_t term = 1, pe = 1;
            while (t % p == 0) {
                t /= p;
                pe *= p;
                term += pe;
            }
            rem[idx] = t;
            sig[idx] *= term;
            sf[idx] *= (p + 1);
        }
    }
    for (i = 0; i < B; i++) {
        uint64_t k = lo + 2 * (uint64_t)i;
        uint64_t s = sig[i], f = sf[i];
        if (rem[i] > 1) {
            s *= rem[i] + 1;
            f *= rem[i] + 1;
        }
        if (s - f > 2 * k) {
            if (A357607_cnt == A357607_cap) {
                A357607_cap = A357607_cap ? A357607_cap * 2 : 1024;
                A357607_terms = (uint64_t *)realloc(A357607_terms, A357607_cap * sizeof(uint64_t));
            }
            A357607_terms[A357607_cnt++] = k;
        }
    }
    free(comp);
    free(rem);
    free(sig);
    free(sf);
    A357607_next = hi;
}

seq_t a(idx_t n)
{
    while (A357607_cnt < (size_t)n) A357607_block();
    return (seq_t)A357607_terms[n - 1];
}
