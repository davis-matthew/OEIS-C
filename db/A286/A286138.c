/*
 * A286138  Pseudo-palindromic numbers: not palindromes (A002113), but a
 *          nontrivial palindromic concatenation (AA or ABA) of arbitrary
 *          nonzero integers A and B.
 * https://oeis.org/A286138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   concat_parts, digits, ensure_capacity, is_palindrome, pow10,
 *            seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This program generates the OEIS sequence A286138: pseudo‑palindromic
    numbers that are not themselves palindromes.  A term is the smallest
    integer that can be written as a concatenation AA or ABA (with
    non‑zero integer strings A and B), is not a palindrome, and is
    greater than all previously computed terms.  The implementation
    builds all admissible numbers of increasing decimal length and
    keeps them sorted to answer queries in O(1) after the necessary
    precomputation. */

/* Required utilities ---------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/concat_parts.h"   /* seq_t concat_parts(idx_t a, idx_t b) */
#include "../utils/is_palindrome.h"  /* bool is_palindrome(seq_t, idx_t base) */
#include "../utils/ensure_capacity.h"/* void ensure_capacity(size_t, size_t*, void**, size_t) */
#include "../utils/seq_cmp.h"        /* int seq_cmp(const void*, const void*) */
#include "../utils/digits.h"         /* int digits(idx_t x) */
#include "../utils/pow10.h"          /* seq_t pow10(idx_t e) */

/* Global state --------------------------------------------------------- */
static seq_t *seq_buf = NULL;      /* computed sequence values */
static size_t seq_cap = 0;         /* buffer capacity */
static size_t seq_len = 0;         /* number of terms stored */
static idx_t  curr_len = 2;        /* smallest decimal length to generate */

/* ---------------------------------------------------------------------- */
static void generate_length(idx_t len)
{
    /* Local buffer for candidates of this length. */
    seq_t *local = NULL;
    size_t local_cap = 0;
    size_t local_len = 0;

    /* ------------------------------------------------------------------
       1. AA case: only if len is even.
       ------------------------------------------------------------------ */
    if ((len & 1) == 0) {
        idx_t m = len / 2;                       /* digits in A */
        uint64_t start = 1;
        for (idx_t i = 1; i < m; ++i) start *= 10;   /* 10^(m-1) */
        uint64_t end = start * 10 - 1;          /* 10^m - 1 */

        seq_t pow10m = pow10(m);

        for (uint64_t A = start; A <= end; ++A) {
            seq_t v = (seq_t)A * pow10m + (seq_t)A;
            if (!is_palindrome(v, 10)) {
                if (local_len == local_cap)
                    ensure_capacity(local_len + 1, &local_cap,
                                     (void **)&local, sizeof(seq_t));
                local[local_len++] = v;
            }
            if (A == end) break;   /* protection against overflow */
        }
    }

    /* ------------------------------------------------------------------
       2. ABA case: any length.
       ------------------------------------------------------------------ */
    for (idx_t m = 1; (2 * m + 1) <= len; ++m) {
        idx_t k = len - 2 * m;           /* digits in B  */
        /* ranges for A and B */
        uint64_t startA = 1;
        for (idx_t i = 1; i < m; ++i) startA *= 10;
        uint64_t endA = startA * 10 - 1;

        uint64_t startB = 1;
        for (idx_t i = 1; i < k; ++i) startB *= 10;
        uint64_t endB = startB * 10 - 1;

        seq_t pow10m = pow10(m);
        seq_t pow10m_k = pow10(m + k);

        for (uint64_t A = startA; A <= endA; ++A) {
            for (uint64_t B = startB; B <= endB; ++B) {
                seq_t v = (seq_t)A * pow10m_k    /* A at the front   */
                        + (seq_t)B * pow10m      /* B in the middle  */
                        + (seq_t)A;              /* A at the back    */
                if (!is_palindrome(v, 10)) {
                    if (local_len == local_cap)
                        ensure_capacity(local_len + 1, &local_cap,
                                         (void **)&local, sizeof(seq_t));
                    local[local_len++] = v;
                }
                if (B == endB) break;
            }
            if (A == endA) break;
        }
    }

    /* If any candidates were produced, sort and append to the global
       sequence. */
    if (local_len > 0) {
        qsort(local, local_len, sizeof(seq_t), seq_cmp);

        if (seq_len + local_len > seq_cap)
            ensure_capacity(seq_len + local_len, &seq_cap,
                            (void **)&seq_buf, sizeof(seq_t));

        for (size_t i = 0; i < local_len; ++i)
            seq_buf[seq_len + i] = local[i];
        seq_len += local_len;
    }

    /* Clean up local buffer. */
    if (local_cap > 0)
        free(local);
}

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* 1‑based indexing.  Compute enough terms to answer the request. */
    while (seq_len < (size_t)n) {
        ++curr_len;
        generate_length(curr_len);
    }
    return seq_buf[(size_t)n - 1];
}
