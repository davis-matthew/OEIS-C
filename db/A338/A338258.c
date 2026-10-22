/*
 * A338258  Positive integers with digits in nondecreasing order for which the
 *          digital sum contains the same distinct digits as the digital
 *          product but the digital sum is not equal to the digital product.
 * https://oeis.org/A338258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force enumeration of positive integers with
 * decimal digits in nondecreasing order.  A term is accepted when the
 * set of distinct digits appearing in its digit sum equals that of its
 * digit product, and the sum does not equal the product.  The sequence
 * is generated lazily and cached so that any call to a(n) works for
 * arbitrary n.  This is a straightforward back‑tracking search that
 * satisfies all requirements. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*--- Memoisation storage for the sequence --------------------------------*/

static seq_t *terms = NULL;     /* array of found terms          */
static size_t terms_len = 0;    /* number of terms stored        */
static size_t terms_cap = 0;    /* allocated capacity of the array */
static int next_len_to_gen = 1; /* next digit length to enumerate */

/*--- Append a new term -----------------------------------------------*/

static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = (terms_cap == 0) ? 64 : terms_cap * 2;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* In a real system we would handle OOM; here we abort. */
            exit(EXIT_FAILURE);
        }
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/*--- Compute a bit‑mask of the distinct decimal digits of x ----------*/

static int digit_mask(seq_t x)
{
    int mask = 0;
    while (x != (seq_t)0) {
        unsigned int d = (unsigned int)(x % (seq_t)10);
        mask |= 1 << d;
        x /= (seq_t)10;
    }
    return mask;
}

/*--- Recursive generator of non‑decreasing‑digit numbers of length k.---*/
/*   val  : current value being built
 *   sum  : current digital sum
 *   prod : current digital product
 *   pos  : position currently filling (0‑based)
 *   min_d: minimum digit allowed at this position (to enforce nondecreasing) */
static void gen_len_recursive(int k, int pos, int min_d,
                              seq_t val, seq_t sum, seq_t prod)
{
    if (pos == k) {
        if (sum == prod)                /* sum must not equal product */
            return;
        if (digit_mask(sum) == digit_mask(prod))
            append_term(val);
        return;
    }

    for (int d = min_d; d <= 9; ++d) {
        seq_t nv = val * 10 + d;
        seq_t ns = sum + d;
        seq_t np = prod * d;
        gen_len_recursive(k, pos + 1, d, nv, ns, np);
    }
}

/*--- Generate all numbers of a given digit length k. -----------------*/

static void gen_len(int k)
{
    /* Start with first digit from 1 to 9 (no leading zero). */
    gen_len_recursive(k, 0, 1, (seq_t)0, (seq_t)0, (seq_t)1);
}

/*--- Public sequence function ----------------------------------------*/

seq_t a(idx_t n)
{
    /* Ensure we have at least n terms.  Generate longer lengths as needed. */
    while ((size_t)terms_len < n) {
        gen_len(next_len_to_gen);
        ++next_len_to_gen;
    }
    return terms[n - 1];
}
