/*
 * A373487  Numbers k such that A276085(k) is a multiple of A003415(k), where
 *          A003415 is the arithmetic derivative and A276085 is fully additive
 *          with a(p) = p#/p.
 * https://oeis.org/A373487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   search for k with A276085(k) == 0 (mod A003415(k)); A276085 is
 *            fully additive with a(p) = p#/p, evaluated modulo A003415(k),
 *            and k = 1 is included
 * Range:     u8 n <= 68, u16 n <= 8434, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9578 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static uint64_t *A373487_pr = NULL;
static size_t A373487_npr = 0, A373487_cap = 0;
static uint64_t *A373487_terms = NULL;
static size_t A373487_tcnt = 0, A373487_tcap = 0;
static uint64_t A373487_cur = 1;

static void A373487_add_prime(uint64_t p) {
    if (A373487_npr == A373487_cap) {
        A373487_cap = A373487_cap ? A373487_cap * 2 : 1024;
        A373487_pr = (uint64_t *)realloc(A373487_pr, A373487_cap * sizeof(uint64_t));
        if (!A373487_pr) abort();
    }
    A373487_pr[A373487_npr++] = p;
}

static void A373487_add_term(uint64_t k) {
    if (A373487_tcnt == A373487_tcap) {
        A373487_tcap = A373487_tcap ? A373487_tcap * 2 : 1024;
        A373487_terms = (uint64_t *)realloc(A373487_terms, A373487_tcap * sizeof(uint64_t));
        if (!A373487_terms) abort();
    }
    A373487_terms[A373487_tcnt++] = k;
}

seq_t a(idx_t n) {
    while (A373487_tcnt < (size_t)n) {
        uint64_t k = A373487_cur++;
        if (k == 1) { A373487_add_term(1); continue; }
        uint64_t fac[20], ex[20];
        int nf = 0;
        uint64_t rem = k;
        for (size_t i = 0; i < A373487_npr; ++i) {
            uint64_t p = A373487_pr[i];
            if (p * p > rem) break;
            if (rem % p == 0) {
                int e = 0;
                while (rem % p == 0) { rem /= p; ++e; }
                fac[nf] = p; ex[nf] = (uint64_t)e; ++nf;
            }
        }
        if (rem > 1) { fac[nf] = rem; ex[nf] = 1; ++nf; }
        if (nf == 1 && ex[0] == 1) {
            A373487_add_prime(k);
            A373487_add_term(k);
            continue;
        }
        uint64_t d = 0;
        for (int i = 0; i < nf; ++i) d += (k / fac[i]) * ex[i];
        uint64_t prod = 1 % d, sum = 0;
        size_t idx = 0;
        for (int i = 0; i < nf && prod != 0; ++i) {
            while (idx < A373487_npr && A373487_pr[idx] < fac[i] && prod != 0) {
                prod = prod * (A373487_pr[idx] % d) % d;
                ++idx;
            }
            sum = (sum + (ex[i] % d) * prod) % d;
        }
        if (sum % d == 0) A373487_add_term(k);
    }
    return (seq_t)A373487_terms[(size_t)n - 1];
}
