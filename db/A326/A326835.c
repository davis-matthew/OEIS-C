/*
 * A326835  Numbers whose divisors have distinct values of the Euler totient
 *          function (A000010).
 * https://oeis.org/A326835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9580 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A326835: numbers k whose proper divisors have distinct Euler
   totient values. This implementation incrementally tests odd integers
   for the property, memoises the found terms, and returns the n‑th
   term on demand.  It works for both C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

/* Generate all phi(d) for d | k, given the prime factorisation of k.
   primes[0..nf-1] and exps[0..nf-1] hold the distinct prime factors and
   their exponents in k.  The result is accumulated in phis[]. */
static void gather_gen(int idx, seq_t cur_phi,
                       const uint64_t primes[], const int exps[],
                       int nf, seq_t *phis, size_t *cnt)
{
    if (idx == nf) {
        phis[*cnt] = cur_phi;
        (*cnt)++;
        return;
    }
    /* option 0: this prime does not appear in the divisor */
    gather_gen(idx + 1, cur_phi, primes, exps, nf, phis, cnt);

    /* options 1..exps[idx] */
    seq_t mult = 1;                     /* p^(e-1) */
    seq_t phi_mul = (seq_t)(primes[idx] - 1);
    for (int e = 1; e <= exps[idx]; ++e) {
        seq_t new_phi = cur_phi * mult * phi_mul;
        gather_gen(idx + 1, new_phi, primes, exps, nf, phis, cnt);
        mult *= (seq_t)primes[idx];
    }
}

/* Test whether a given integer k satisfies the distinct phi condition. */
static bool is_term(uint64_t k)
{
    if (k == 0) return false;
    /* factorisation of k */
    uint64_t primes[16];
    int exps[16];
    int nf = 0;
    uint64_t t = k;
    if (t % 2 == 0) {
        int cnt = 0;
        while (t % 2 == 0) { t /= 2; cnt++; }
        primes[nf] = 2; exps[nf++] = cnt;
    }
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            int cnt = 0;
            while (t % p == 0) { t /= p; cnt++; }
            primes[nf] = p; exps[nf++] = cnt;
        }
    }
    if (t > 1) {
        primes[nf] = t; exps[nf++] = 1;
    }

    seq_t phis[256];
    size_t pcnt = 0;
    gather_gen(0, (seq_t)1, primes, exps, nf, phis, &pcnt);

    mem_qsort(phis, pcnt, sizeof(seq_t), cmp_seq_t);
    for (size_t i = 1; i < pcnt; ++i) {
        if (phis[i] == phis[i - 1]) return false;
    }
    return true;
}

/* Return the n-th term of the sequence (n ≥ 1).  The function
   memoises already found terms and builds them lazily. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, handled with a zero return */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint64_t next_k = 1;

    while (terms_len < (size_t)n) {
        uint64_t k = next_k++;
        if (k > 1 && (k & 1ULL) == 0) continue; /* skip evens >1 */
        if (is_term(k)) {
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap << 1 : 16;
                seq_t *new_ptr = mem_resize(seq_t, terms, new_cap);
                if (!new_ptr) exit(1);
                terms = new_ptr;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)k;
        }
    }
    return terms[n - 1];
}
