/*
 * A343148  Numbers k such that A083266(k) is prime.
 * https://oeis.org/A343148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that A083266(k) = sigma(k) + k*phi(k)/2
 *            (k>1) - 1 is prime; found by incremental search with cached
 *            terms
 * Range:     u8 n <= 7, u16 n <= 1449, u32 n <= 9985, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9986 (2026-11-10)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

static seq_t A083266_a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    idx_t temp = n;
    seq_t sigma = 1;
    seq_t phi = n_t;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            seq_t pw = 1;
            seq_t seg = 1;
            while (temp % p == 0) {
                temp /= p;
                pw *= (seq_t)p;
                seg += pw;
            }
            sigma *= seg;
            phi = phi / (seq_t)p * ((seq_t)p - 1);
        }
    }
    if (temp > 1) {
        seq_t p_val = (seq_t)temp;
        seq_t seg = 1 + p_val;
        sigma *= seg;
        phi = phi / p_val * (p_val - 1);
    }
    seq_t phi_sum;
    if (n_t == 1) {
        phi_sum = 1;
    } else {
        phi_sum = (n_t * phi) / 2;
    }
    return sigma + phi_sum - 1;
}

static idx_t *A343148_terms = NULL;
static idx_t A343148_count = 0;
static idx_t A343148_cap = 0;
static idx_t A343148_next = 1;

seq_t a(idx_t n)
{
    while (A343148_count < n) {
        idx_t k = A343148_next++;
        if (is_prime_seq(A083266_a(k))) {
            if (A343148_count == A343148_cap) {
                A343148_cap = A343148_cap ? A343148_cap * 2 : 1024;
                A343148_terms = (idx_t *)realloc(A343148_terms, (size_t)A343148_cap * sizeof(idx_t));
            }
            A343148_terms[A343148_count++] = k;
        }
    }
    return (seq_t)A343148_terms[n - 1];
}
