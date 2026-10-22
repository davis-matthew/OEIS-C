/*
 * A339714  Numbers with equal number of even and odd digits such that the sum
 *          a(n) + a(n+1) has an equal number of even and odd digits too.
 * https://oeis.org/A339714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   digit_counts, digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A339714: greedy lexicographically earliest sequence of distinct
   positive integers a(n) such that
   – each a(n) has an equal number of even and odd decimal digits
     (hence an even number of digits),
   – each adjacent sum a(n)+a(n+1) also has equal even/odd digit counts.
   The first term a(1) is the smallest integer with the required
   digit property; subsequent terms are the smallest integers not
   previously used that satisfy the adjacency condition with the
   preceding term. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"   /* void digit_counts(seq_t x, unsigned char cnt[10]); */
#include "../utils/digit_len.h"      /* int digit_len(seq_t x); */
#include "../utils/pow10.h"          /* seq_t pow10(idx_t e); */

/* Test whether x has an equal number of even and odd decimal digits. */
static bool
has_equal_even_odd(seq_t x)
{
    if (x == 0)           /* 0 has one digit, not equal */
        return false;

    unsigned char cnt[10];
    digit_counts(x, cnt);

    unsigned ev = 0, od = 0;
    for (int d = 0; d < 10; ++d) {
        if (d % 2 == 0)
            ev += cnt[d];
        else
            od += cnt[d];
    }
    return ev == od;
}

/* Pre‑allocated storage for the sequence. */
static seq_t *seq   = NULL;   /* array holding the terms */
static size_t seq_len = 0;    /* number of terms currently stored */
static size_t seq_cap = 0;    /* current capacity of the array */

/* Find the next term after prev, using the current seq array for
   uniqueness testing. */
static seq_t
next_term(seq_t prev)
{
    seq_t cand = 1;          /* start from the smallest positive integer */

    while (true) {
        /* Skip numbers already used */
        bool used = false;
        for (size_t i = 0; i < seq_len; ++i) {
            if (seq[i] == cand) { used = true; break; }
        }
        if (used) { ++cand; continue; }

        /* Must have equal even/odd digits themselves */
        if (!has_equal_even_odd(cand)) { ++cand; continue; }

        /* For n>1 require previous sum also has the property */
        if (seq_len > 0) {
            seq_t sum = prev + cand;
            if (!has_equal_even_odd(sum)) { ++cand; continue; }
        }

        /* This candidate is valid */
        return cand;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)                /* out of range – by definition return 0 */
        return (seq_t)0;

    /* Grow the storage as needed */
    while (seq_len < (size_t)n) {
        /* Allocate or extend */
        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
            seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            seq_cap = new_cap;
        }

        seq_t prev = (seq_len == 0) ? (seq_t)0 : seq[seq_len - 1];
        seq[seq_len++] = next_term(prev);
    }

    return seq[n - 1];
}
