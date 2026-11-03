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
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   is_palindrome_base, mem, num_digits_idx
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

#include <stddef.h>
#include "../utils/is_palindrome_base.h"
#include "../utils/num_digits_idx.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#endif

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size) /* elem_size: sizeof(T) */
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, new_cap);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

static int seq_cmp(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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
            if (!is_palindrome_base(v, 10)) {
                if (local_len == local_cap)
                    ensure_capacity(local_len + 1, &local_cap,
                                     OEISC_VPP(&local), sizeof(seq_t));
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
                if (!is_palindrome_base(v, 10)) {
                    if (local_len == local_cap)
                        ensure_capacity(local_len + 1, &local_cap,
                                         OEISC_VPP(&local), sizeof(seq_t));
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
        mem_qsort(local, local_len, sizeof(seq_t), seq_cmp);

        if (seq_len + local_len > seq_cap)
            ensure_capacity(seq_len + local_len, &seq_cap,
                            OEISC_VPP(&seq_buf), sizeof(seq_t));

        for (size_t i = 0; i < local_len; ++i)
            seq_buf[seq_len + i] = local[i];
        seq_len += local_len;
    }

    /* Clean up local buffer. */
    if (local_cap > 0)
        mem_free(local);
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
