/*
 * A370975  a(n) = (A370952(n+1) - A370952(n)) /(2*n+1).
 * https://oeis.org/A370975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (A370952(n+1) - A370952(n))/(2*n+1), where A370952(1) = 1
 *            and A370952(n) is the least positive integer not already used
 *            with A370952(n) == A370952(n-1) (mod 2*n-1) (computed by search)
 * Range:     i8 n <= 25, i16 n <= 4669, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9561 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static void A370952_ensure_capacity(seq_t **arr, size_t *cap, size_t *len)
{
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
}

static seq_t A370952_a(idx_t n)
{
    static seq_t *terms    = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    static seq_t *used     = NULL;
    static size_t used_len  = 0;
    static size_t used_cap  = 0;

    if (terms_len == 0) {
        terms_cap = 16;
        used_cap  = 16;
        terms = mem_new(seq_t, terms_cap);
        used  = mem_new(seq_t, used_cap);
        terms_len = 1;
        used_len  = 1;
        terms[0] = (seq_t)1;
        used[0]  = (seq_t)1;
    }

    for (idx_t i = (idx_t)terms_len + 1; i <= n; ++i) {
        seq_t prev = terms[i - 2];
        seq_t m    = ((seq_t)i * 2) - 1;

        seq_t r = prev % m;
        if (r == (seq_t)0)
            r = m;
        seq_t cand = r;

        for (;;) {
            bool used_flag = false;
            for (size_t j = 0; j < used_len; ++j) {
                if (cand == used[j]) { used_flag = true; break; }
            }
            if (!used_flag) break;
            cand += m;
        }

        A370952_ensure_capacity(&terms, &terms_cap, &terms_len);
        terms[terms_len++] = cand;

        A370952_ensure_capacity(&used, &used_cap, &used_len);
        used[used_len++] = cand;
    }

    return terms[n - 1];
}

seq_t a(idx_t n)
{
    seq_t hi = A370952_a(n + 1);
    seq_t lo = A370952_a(n);
    return (hi - lo) / ((seq_t)2 * (seq_t)n + 1);
}
