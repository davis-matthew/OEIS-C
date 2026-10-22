/*
 * A368955  Numbers that are the product of two repdigit numbers.
 * https://oeis.org/A368955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = i*j*(10^k-1)*(10^m-1)/81 (k,m ≥ 0, 0 ≤ i,j ≤ 9)
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   max_rep_len_for_type, pow10, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* malloc, free, qsort */
#include <stdint.h>          /* uintptr_t for casts if needed */
#include "../utils/pow10.h"  /* seq_t pow10(idx_t) */
#include "../utils/seq_cmp.h" /* int seq_cmp(const void *, const void *) */
#include "../utils/max_rep_len_for_type.h"

static seq_t *seq_data = NULL;      /* pointer to the generated sorted list */
static size_t seq_len  = 0;          /* number of unique values stored */

/* Generate the full sequence once. */
static void generate_sequence(void)
{
    if (seq_data) return;                 /* already generated */

    const idx_t L = max_rep_len_for_type();               /* repdigit length */
    const size_t max_candidates = (size_t)L * (size_t)L * 81 + 1; /* +1 for zero */

    seq_data = (seq_t*)malloc(max_candidates * sizeof(seq_t));
    if (!seq_data) exit(1);
    seq_len = 0;
    seq_data[seq_len++] = (seq_t)0;        /* zero product (k or m == 0) */

    if (sizeof(seq_t) <= 8) { /* 64‑bit or smaller, protect against overflow */
        const __uint128_t max_seq = ((__uint128_t)~(seq_t)0);
        for (idx_t k = 1; k <= L; ++k) {
            seq_t powk = pow10(k);
            for (idx_t m = 1; m <= L; ++m) {
                seq_t powm = pow10(m);
                __uint128_t term1 = (__uint128_t)(powk - (seq_t)1);
                __uint128_t term2 = (__uint128_t)(powm - (seq_t)1);
                __uint128_t t = (term1 * term2) / 81;          /* base value */

                for (int i = 1; i <= 9; ++i) {
                    for (int j = 1; j <= 9; ++j) {
                        __uint128_t prod = t * (i * j);
                        if (prod > max_seq) continue;          /* skip overflow */
                        seq_data[seq_len++] = (seq_t)prod;
                    }
                }
            }
        }
    } else { /* arbitrary precision: overflow cannot occur */
        for (idx_t k = 1; k <= L; ++k) {
            seq_t powk = pow10(k);
            for (idx_t m = 1; m <= L; ++m) {
                seq_t powm = pow10(m);
                seq_t base = ((powk - (seq_t)1) * (powm - (seq_t)1)) / (seq_t)81;
                for (int i = 1; i <= 9; ++i) {
                    for (int j = 1; j <= 9; ++j) {
                        seq_data[seq_len++] = base * (seq_t)(i * j);
                    }
                }
            }
        }
    }

    /* Sort and remove duplicates in place. */
    qsort(seq_data, seq_len, sizeof(seq_t), seq_cmp);
    size_t uniq = 1;
    for (size_t i = 1; i < seq_len; ++i) {
        if (seq_data[i] != seq_data[uniq - 1]) {
            seq_data[uniq++] = seq_data[i];
        }
    }
    seq_len = uniq;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (!seq_data) generate_sequence();
    if ((idx_t)n > seq_len) return (seq_t)0;    /* out of range */
    return seq_data[n - 1];
}
