/*
 * A347336  Lexicographically earliest sequence of distinct positive integers
 *          such that the concatenation of a(n) and a(n+1) added to a(n+2) is
 *          a palindrome in base 10.
 * https://oeis.org/A347336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   digit_len, is_palindrome, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: lexicographically earliest sequence of distinct positive integers such
   that the concatenation of a(n) and a(n+1) added to a(n+2) is a palindrome
   (OEIS A347336).  The sequence is generated greedily: each new term is the
   smallest unused positive integer that satisfies the palindrome condition.
   The first two terms are 1 and 2. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* External helper functions supplied by the build system */
#include "../utils/digit_len.h"        /* int digit_len(idx_t) */
#include "../utils/pow10.h"           /* seq_t pow10(idx_t) */
#include "../utils/is_palindrome.h"   /* bool is_palindrome(seq_t, idx_t) */

/* Internal dynamic arrays for the sequence and the set of used values */
static seq_t *seq      = NULL;   /* stores a(1)...a(seq_len)  (index 0 = a(1)) */
static size_t seq_len  = 0;      /* number of terms currently stored */
static size_t seq_cap  = 0;      /* allocated capacity of seq[] */

static seq_t *used     = NULL;   /* list of all values already used in the sequence */
static size_t used_len = 0;      /* number of values currently stored */
static size_t used_cap = 0;      /* allocated capacity of used[] */

/* Ensure the 'seq' array has at least 'need' capacity */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap < need) {
        size_t new_cap = seq_cap == 0 ? 16 : seq_cap << 1;
        while (new_cap < need) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* Ensure the 'used' array has at least 'need' capacity */
static void ensure_used_cap(size_t need)
{
    if (used_cap < need) {
        size_t new_cap = used_cap == 0 ? 16 : used_cap << 1;
        while (new_cap < need) new_cap <<= 1;
        used = (seq_t *)realloc(used, new_cap * sizeof(seq_t));
        used_cap = new_cap;
    }
}

/* Linear search whether val has already appeared in the sequence */
static bool used_exists(seq_t val)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used[i] == val) return true;
    return false;
}

/* Append val to the used array */
static void used_insert(seq_t val)
{
    ensure_used_cap(used_len + 1);
    used[used_len++] = val;
}

/* Generate terms up to index n (1‑based) if required, then return a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined but safe */

    /* Initialize the sequence on first call */
    if (seq_len == 0) {
        ensure_seq_cap(2);
        seq[0] = (seq_t)1;                /* a(1) = 1 */
        seq[1] = (seq_t)2;                /* a(2) = 2 */
        seq_len = 2;

        ensure_used_cap(2);
        used[0] = (seq_t)1;
        used[1] = (seq_t)2;
        used_len = 2;
    }

    /* Generate terms until we reach n */
    while (seq_len < (size_t)n) {
        /* We are computing a(m) where m = seq_len + 1 (1‑based) */
        seq_t a_m2 = seq[seq_len - 2];    /* a(m-2) */
        seq_t a_m1 = seq[seq_len - 1];    /* a(m-1) */

        /* Build concatenation a(m-2) || a(m-1) */
        int dig_len_m1 = digit_len((idx_t)a_m1);
        seq_t pow10v   = pow10((idx_t)dig_len_m1);
        seq_t base     = a_m2 * pow10v + a_m1;   /* a(m-2)||a(m-1) */

        /* Find the smallest unused positive integer giving a palindrome */
        seq_t cand = (seq_t)1;
        while (true) {
            if (!used_exists(cand)) {
                seq_t sum = base + cand;
                if (is_palindrome(sum, (idx_t)10))
                    break;
            }
            ++cand;
        }

        /* Store the new term and mark it as used */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len++] = cand;
        used_insert(cand);
    }

    return seq[n - 1];
}
