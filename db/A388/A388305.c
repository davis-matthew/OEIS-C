/*
 * A388305  Numbers divisible by at least 3 primes where not all exponents in
 *          the factorization are equal.
 * https://oeis.org/A388305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: A388305 – numbers divisible by at least 3 distinct primes
 *                where not all prime exponents in the factorization are equal.
 * This function supplies the n-th element (n ≥ 1) of the sequence.
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* table of computed terms */
static seq_t *terms = NULL;      /* array of sequence values */
static idx_t  terms_cnt = 0;     /* number of terms currently stored */
static idx_t  terms_cap = 0;     /* allocated capacity of terms array */

/* next integer to test (starts at 2, but we skip below 60) */
static uint64_t next_candidate = 2;

/* enlarge the terms array to at least new_cap elements */
static void grow_terms(idx_t new_cap)
{
    if (new_cap <= terms_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(terms, (size_t)new_cap * sizeof(seq_t));
    if (!tmp && new_cap != 0)
        abort();  /* out‑of‑memory */
    terms = tmp;
    terms_cap = new_cap;
}

/* test whether x satisfies the definition of A388305 */
static bool qualifies(uint64_t x)
{
    if (x < 60)                /* the first term is 60 */
        return false;

    uint64_t primes[16];       /* enough for typical numbers below 1e12 */
    uint64_t exps [16];
    size_t   cnt;

    factor_u64(x, primes, exps, &cnt);   /* provided routine */

    if (cnt < 3)                /* need at least 3 distinct primes */
        return false;

    bool all_equal = true;
    for (size_t i = 1; i < cnt; ++i) {
        if (exps[i] != exps[0]) {
            all_equal = false;
            break;
        }
    }
    return !all_equal;          /* exclude the all‑exponent‑equal cases */
}

/* ensure that at least n terms are pre‑computed */
static void ensure_terms(idx_t n)
{
    while (terms_cnt < n) {
        if (next_candidate < 60)
            next_candidate = 60;

        if (qualifies(next_candidate)) {
            if (terms_cap == 0 || terms_cnt == terms_cap)
                grow_terms(terms_cap ? terms_cap * 2 : 64);
            terms[terms_cnt++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* the required external function */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n=0; return 0 as placeholder */
        return 0;
    ensure_terms(n);
    return terms[n - 1];
}
