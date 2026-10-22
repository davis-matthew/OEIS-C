/*
 * A334392  Numbers m such that the LCM of their palindromic divisors is
 *          neither 1 nor m.
 * https://oeis.org/A334392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 138, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_palint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A334392: numbers m such that the LCM of their palindromic divisors is neither 1 nor m.
   The function builds the sequence on demand, caching the results.
   It uses a simple palindrome test and generates all palindromic numbers up
   to the current search bound once, re‑using them for subsequent values. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/is_palint.h"

/* ---------- Helper: palindrome test (decimal) ---------- */

/* ---------- Palindrome cache ---------- */

static seq_t *pal_list = NULL;        /* sorted array of palindromes */
static size_t pal_len  = 0;
static size_t pal_cap  = 0;
static uint64_t pal_max = 0;          /* maximum integer for which
                                        pal_list has been generated */

static void ensure_pal_list(uint64_t limit)
{
    if (limit <= pal_max) return;

    /* grow the array as needed */
    for (uint64_t i = pal_max + 1; i <= limit; ++i) {
        if (is_palint((seq_t)i)) {
            if (pal_len == pal_cap) {
                pal_cap = pal_cap ? pal_cap * 2 : 16;
                pal_list = (seq_t *)realloc(pal_list,
                                            pal_cap * sizeof(seq_t));
            }
            pal_list[pal_len++] = (seq_t)i;
        }
    }
    pal_max = limit;
}

/* ---------- Sequence cache ---------- */

static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static idx_t last_checked = 0;

static void seq_ensure_capacity(size_t needed)
{
    if (seq_cap < needed) {
        seq_cap = seq_cap ? seq_cap * 2 : 16;
        while (seq_cap < needed) seq_cap *= 2;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }
}

/* ---------- Test whether a number satisfies the definition ---------- */

static bool is_in_sequence(idx_t m)
{
    /* Ensure we have palindromes up to m */
    ensure_pal_list((uint64_t)m);

    seq_t l = 1;                            /* current LCM */
    seq_t mseq = (seq_t)m;
    for (size_t i = 0; i < pal_len && pal_list[i] <= mseq; ++i) {
        seq_t d = pal_list[i];
        if (mseq % d == 0) {
            seq_t g = gcd(l, d);
            l = (l / g) * d;
            if (l == mseq) break;           /* early exit */
        }
    }
    return l != 1 && l != mseq;
}

/* ---------- Main function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1 */

    while (seq_len < (size_t)n) {
        ++last_checked;
        if (is_in_sequence(last_checked)) {
            seq_ensure_capacity(seq_len + 1);
            seq[seq_len++] = (seq_t)last_checked;
        }
    }
    return seq[n - 1];
}
