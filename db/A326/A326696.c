/*
 * A326696  Numbers m with at least one divisor d > 1 such that sigma(d)
 *          divides m.
 * https://oeis.org/A326696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A326696 – numbers m with at least one divisor d>1 such that
 *            sigma(d) divides m
 * Range:     u8 n <= 43, u16 n <= 6214, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   dfs_has_divisor, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/dfs_has_divisor.h"

/* Memoised storage of the sequence */
static seq_t *terms      = NULL;
static size_t terms_len  = 0;
static size_t terms_cap  = 0;
static idx_t  next_check = 2;

/* Ensure the terms array has room for one more element */
static void ensure_terms_capacity(void)
{
    if (terms_cap == 0) {
        terms_cap = 16;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    } else if (terms_len == terms_cap) {
        terms_cap <<= 1;
        terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
    }
}

/* Return true iff m has a divisor d>1 with sigma(d) | m. */
static bool has_good_divisor(idx_t m)
{
    seq_t m_seq = (seq_t)m;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(m, primes, exps, &cnt);
    return dfs_has_divisor(m_seq, primes, exps, cnt, 0, 1, 1);
}

/* Generate terms until at least n terms are stored. */
static void generate_up_to(idx_t n)
{
    while (terms_len < n) {
        if (has_good_divisor(next_check)) {
            ensure_terms_capacity();
            terms[terms_len++] = (seq_t)next_check;
        }
        ++next_check;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n<1; return 0 */
    generate_up_to(n);
    return terms[n - 1];
}
