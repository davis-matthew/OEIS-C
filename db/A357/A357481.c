/*
 * A357481  a(n) is the least integer b such that the digit representation of
 *          n in base b is equal to the digit representation in base b of the
 *          initial terms of the sets of divisors of n in increasing order, or
 *          -1 if no such b exists.
 * https://oeis.org/A357481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) is computed by searching for the smallest base b such that
 *            the base‑b representation of n equals the concatenation of the
 *            base‑b representations of the initial divisors of n. If no such
 *            base exists, a(n) returns -1
 * Range:     i8 n <= 129, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   cmp_idx, factor_u64, number_to_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/cmp_idx.h"
#include "../utils/number_to_digits.h"

/* Comparator for qsort on idx_t values. */

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Special case n = 1: 1 in base 2 is "1", matching divisor 1 */
    if (n == 1) return (seq_t)2;

    /* Factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt = 0;
    factor_u64(n, primes, exps, &pf_cnt);

    /* Generate all divisors of n */
    size_t cap = 16;
    size_t sz  = 0;
    idx_t *divs = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!divs) return (seq_t)-1;  /* allocation failure, unlikely */
    divs[sz++] = 1;  /* divisor 1 always present */

    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        size_t cur_sz = sz;
        idx_t power = 1;
        for (size_t k = 1; k <= e; ++k) {
            power *= (idx_t)p;
            for (size_t j = 0; j < cur_sz; ++j) {
                idx_t val = divs[j] * power;
                if (sz == cap) {
                    cap <<= 1;
                    idx_t *tmp = (idx_t *)realloc(divs, cap * sizeof(idx_t));
                    if (!tmp) { free(divs); return (seq_t)-1; }
                    divs = tmp;
                }
                divs[sz++] = val;
            }
        }
    }

    /* Sort divisors to process them in increasing order */
    qsort(divs, sz, sizeof(idx_t), cmp_idx);

    /* Search for the smallest base b */
    for (idx_t b = 2; b < n; ++b) {
        idx_t digits_n[128];
        size_t len_n = number_to_digits(n, b, digits_n);
        if (digits_n[0] != 1) continue;          /* first divisor 1 must match */
        size_t pos = 0;
        bool ok = true;
        for (size_t i = 0; i < sz && pos < len_n; ++i) {
            idx_t digits_d[128];
            size_t len_d = number_to_digits(divs[i], b, digits_d);
            if (pos + len_d > len_n) { ok = false; break; }
            for (size_t k = 0; k < len_d; ++k) {
                if (digits_n[pos + k] != digits_d[k]) { ok = false; break; }
            }
            if (!ok) break;
            pos += len_d;
        }
        if (ok && pos == len_n) {
            free(divs);
            return (seq_t)b;
        }
    }

    free(divs);
    return (seq_t)-1;
}
