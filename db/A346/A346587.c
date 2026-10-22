/*
 * A346587  Value of k for incrementally largest value of k/A002322(k), where
 *          A002322 is the Carmichael function.
 * https://oeis.org/A346587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   factor_u64, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A346587 – record values k for which k/λ(k) is larger than all
   previous ratios.  λ(k) is the Carmichael function.  The function
   iterates k = 1,2,…, computing λ(k) by prime factorisation and keeps
   the list of record values.  The result is the n‑th record (n≥1). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

/* Compute λ(k) for a 64‑bit integer k.                          */
static seq_t carmichael(idx_t k)
{
    if (k == 1)
        return (seq_t)1;

    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64((uint64_t)k, primes, exps, &cnt);

    seq_t lambda = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        seq_t factor;

        if (p == 2) {
            if (e == 1)
                factor = (seq_t)1;
            else if (e == 2)
                factor = (seq_t)2;
            else
                factor = ((seq_t)1) << (e - 2);      /* 2^(e-2)           */
        } else {
            /* λ(p^e) = p^(e‑1) * (p‑1) for odd primes. */
            factor = pow_uint(p, e - 1) * (seq_t)(p - 1);
        }

        seq_t g = gcd(lambda, factor);
        lambda = (lambda / g) * factor;               /* lcm(lambda,factor) */
    }
    return lambda;
}

/* ---------- record list (dynamic array) & state ---------- */
static seq_t *records = NULL;
static size_t rec_cap = 0;
static size_t rec_len = 0;
static idx_t  next_k  = 2;               /* k to examine next          */
static seq_t  cur_num = (seq_t)1;        /* numerator of current record */
static seq_t  cur_den = (seq_t)1;        /* denominator of current record*/
static bool   initialized = false;

static void grow_records(void)
{
    size_t new_cap = (rec_cap == 0) ? 32 : rec_cap * 2;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    if (records) {
        memcpy(new_arr, records, rec_len * sizeof(seq_t));
        free(records);
    }
    records = new_arr;
    rec_cap = new_cap;
}

/* ---------- main sequence function ----------------------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        grow_records();
        records[0] = (seq_t)1;
        rec_len = 1;
        cur_num = (seq_t)1;
        cur_den = (seq_t)1;
        next_k  = 2;
        initialized = true;
    }

    while (rec_len < (size_t)n) {

        if (rec_len == rec_cap) grow_records();

        seq_t lambda_k = carmichael(next_k);

        /* compare next_k / lambda_k   with   cur_num / cur_den    */
        seq_t lhs = (seq_t)next_k * cur_den;
        seq_t rhs = cur_num * lambda_k;
        if (lhs > rhs) {
            records[rec_len++] = (seq_t)next_k;
            cur_num = (seq_t)next_k;
            cur_den = lambda_k;
        }
        ++next_k;
    }

    return records[(size_t)n - 1];
}
