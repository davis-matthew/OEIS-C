/*
 * A337733  Numbers that can be written as (k + sum of digits of k) for some
 *          k, also as (m + product of digits of m) for some m, and finally as
 *          (q * product of digits of q) for some q.
 * https://oeis.org/A337733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A337733 = numbers that can be written as (k + sumdigits k) and
 *            (m + prodigits m) and (q * prodigits q)
 * Range:     u8 n <= 11, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   digit_prod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implementation brute‑forces the three conditions by scanning numbers 1…B and
   recording all values that satisfy all three.  It rebuilds a larger bound if the
   requested index exceeds the amount of terms already found.  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/digit_prod_u64.h"

/* Helper functions for 64‑bit operands ---------------------------------------------------- */
static inline uint64_t digit_sum_u64(uint64_t n) {
    uint64_t s = 0;
    while (n) {
        s += n % 10;
        n /= 10;
    }
    return s;
}

/* Global state --------------------------------------------------------------------------- */
static uint8_t *s1 = NULL, *s2 = NULL, *s3 = NULL;  /* boolean arrays */
static size_t s_cap = 0;                     /* capacity of the boolean arrays */

static seq_t *results = NULL;          /* Array of found terms */
static size_t res_len = 0;             /* Number of terms stored */
static size_t res_cap = 0;             /* Capacity of results array */
static size_t max_checked = 0;         /* Largest bound processed */

/* Ensure three separate boolean arrays of at least 'sz' bytes -------------------------------- */
static void ensure_bools(size_t sz) {
    if (s_cap < sz) {
        uint8_t *tmp = (uint8_t *)realloc(s1, sz);
        if (!tmp) abort();
        s1 = tmp;
        tmp = (uint8_t *)realloc(s2, sz);
        if (!tmp) abort();
        s2 = tmp;
        tmp = (uint8_t *)realloc(s3, sz);
        if (!tmp) abort();
        s3 = tmp;
        s_cap = sz;
    }
    memset(s1, 0, sz);
    memset(s2, 0, sz);
    memset(s3, 0, sz);
}

/* Append a value to the results array ---------------------------------------------------- */
static void push_result(seq_t v) {
    if (res_len >= res_cap) {
        res_cap = res_cap ? res_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(results, res_cap * sizeof(seq_t));
        if (!tmp) abort();
        results = tmp;
    }
    results[res_len++] = v;
}

/* Compute terms up to at least 'target' indices -------------------------------------------- */
static void build_up_to(idx_t target) {
    if (target <= res_len) return;   /* already enough terms present */

    size_t B = max_checked ? max_checked : 200000;   /* start bound */
    while (true) {
        ensure_bools(B + 1);

        /* Fill the three boolean arrays */
        for (size_t k = 1; k <= B; ++k) {
            uint64_t sum = digit_sum_u64(k);
            uint64_t prod = digit_prod_u64(k);

            size_t v1 = k + sum;
            if (v1 <= B) s1[v1] = 1;

            size_t v2 = k + prod;
            if (v2 <= B) s2[v2] = 1;

            if (prod != 0 && k <= B / prod) {
                size_t v3 = k * prod;
                if (v3 <= B) s3[v3] = 1;
            }
        }

        /* Collect all numbers that satisfy all three representations */
        res_len = 0;               /* discard old results */
        for (size_t i = 1; i <= B; ++i) {
            if (s1[i] && s2[i] && s3[i]) {
                push_result((seq_t)i);
                if (res_len >= (size_t)target) break;
            }
        }

        if (res_len >= (size_t)target) {
            max_checked = B;
            break;                 /* sufficient terms gathered */
        }

        /* Not enough terms: extend the bound and retry */
        B *= 2;
        if (B == 0) B = SIZE_MAX;   /* safeguard from overflow */
    }
}

/* Public sequence function --------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for indices below the offset */
    build_up_to(n);
    return results[n - 1];
}
