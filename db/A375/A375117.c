/*
 * A375117  Irregular triangle of positive integers, read by rows, the
 *          elements of the n-th row being the nonzero remainders, in
 *          increasing order, when the Euclidean algorithm is applied to 2^n-1
 *          and n.
 * https://oeis.org/A375117
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = nonzero remainders of Euclidean algorithm on (2^n‑1, n),
 *            flattened row‑wise
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *terms = NULL;        /* flattened sequence a(2), a(3), …  */
static size_t terms_cap = 0;       /* capacity of terms array          */
static size_t terms_len = 0;       /* number of elements in terms      */
static idx_t current_row = 2;      /* next row to generate             */

/* Fast modular exponentiation: (base^exp) % mod  (mod > 0) */
static uint64_t mod_pow_uint(uint64_t base, idx_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    uint64_t b   = base % mod;
    while (exp) {
        if (exp & 1)
            res = (res * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return res;
}

/* Ensure capacity of the terms array */
static void push_term(seq_t val)
{
    if (terms_len == terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 16;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = val;
}

/* Main function required by the build */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;   /* the sequence is defined for n ≥ 2 only */

    /* Generate rows until we have enough terms */
    while (terms_len < n - 1) {
        idx_t row   = current_row;
        idx_t mod   = row;

        /* Compute r1 = (2^row - 1) mod row */
        uint64_t p = mod_pow_uint(2, row, mod);
        idx_t r1 = (p == 0) ? mod - 1 : p - 1;

        /* Euclidean algorithm remainders (strictly decreasing) */
        seq_t remainders[64];
        int rc = 0;
        remainders[rc++] = (seq_t)r1;

        idx_t a_cur = row;
        idx_t b_cur = r1;
        while (b_cur > 0) {
            idx_t r = a_cur % b_cur;
            if (r == 0) break;
            remainders[rc++] = (seq_t)r;
            a_cur = b_cur;
            b_cur = r;
        }

        /* Append remainders in ascending order (reverse of above) */
        for (int i = rc - 1; i >= 0; --i)
            push_term(remainders[i]);

        ++current_row;
    }

    return terms[n - 2];
}
