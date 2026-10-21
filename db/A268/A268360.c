/*
 * A268360  "Four"ier sequence: there is a unique base b >= 5 such that (n
 *          base b) contains the largest number of 4's.
 * https://oeis.org/A268360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   count4_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute OEIS A268360.
 *  Formula: none – term found by exhaustive search for a unique base
 *  b ≥ 5 yielding the maximum number of digit '4's in the base‑b
 *  representation of n.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count4_digits.h"

/* integer square root for 64‑bit values – binary search algorithm */
static inline idx_t int_sqrt(idx_t n)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1ULL << 62;          /* highest power of four ≤ n */
    while (bit > n) bit >>= 2;
    for (; bit; bit >>= 2) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
    }
    return r;
}

/* returns true iff the base giving the largest number of 4’s is unique */
static bool is_unique_max(idx_t m)
{
    if (m < 5) return false;          /* no digit 4 can appear */

    int maxcnt = -1;
    size_t uniq = 0;                /* number of bases attaining maxcnt */

    idx_t s = int_sqrt(m);

    /* ========== SMALL BASES (b ≤ s) ========== */
    for (idx_t b = 5; b <= s; ++b) {
        int cnt = count4_digits(m, b);
        if (cnt > maxcnt) { maxcnt = cnt; uniq = 1; }
        else if (cnt == maxcnt) ++uniq;
    }

    /* ========== LARGE BASES WITH TWO 4’S ========== */
    if (m >= 4 && (m - 4) % 4 == 0) {
        idx_t b2 = (m - 4) / 4;
        if (b2 >= 5 && b2 > s) {
            if (maxcnt < 2) { maxcnt = 2; uniq = 1; }
            else if (maxcnt == 2) ++uniq;
        }
    }

    /* ========== LARGE BASES WITH ONE 4 (high digit 4) ========== */
    idx_t lb = (m / 5) + 1;
    if (lb <= s) lb = s + 1;
    if (lb < 5) lb = 5;
    idx_t ub = m / 4;
    size_t count_single = 0;
    if (ub >= lb) {
        count_single += (size_t)(ub - lb + 1);

        /* exclude the two‑4 base if it lies in this interval */
        if (m >= 4 && (m - 4) % 4 == 0) {
            idx_t b2 = (m - 4) / 4;
            if (b2 >= lb && b2 <= ub) --count_single;
        }
    }

    /* ========== LARGE BASES WITH ONE 4 (low digit 4) ========== */
    idx_t rem = m - 4;
    for (idx_t d = 1; d * d <= rem; ++d) {
        if (rem % d) continue;
        idx_t b = d;
        if (b >= 5 && b > s) {
            idx_t k = rem / b;
            if (k < b && k != 4) ++count_single;
        }
        /* complementary divisor */
        if (d * d == rem) continue;
        idx_t b2 = rem / d;
        if (b2 >= 5 && b2 > s) {
            idx_t k = rem / b2;   /* same as d */
            if (k < b2 && k != 4) ++count_single;
        }
    }

    if (count_single > 0) {
        if (maxcnt < 1) { maxcnt = 1; uniq = count_single; }
        else if (maxcnt == 1) uniq += count_single;
    }

    return uniq == 1;
}

/* ---------- memoised storage for the sequence ---------- */
static seq_t *terms     = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;
static idx_t  last_checked = 0;

static void grow_terms(size_t new_cap)
{
    if (new_cap <= terms_cap) return;
    size_t old_cap = terms_cap;
    terms_cap = new_cap;
    if (terms == NULL)
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    else
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    if (terms == NULL) exit(EXIT_FAILURE);
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset at 1 – behaviour undefined below */

    /* compute terms up to index n if required */
    while (terms_len < n) {
        ++last_checked;                 /* candidate integer to test */

        if (is_unique_max(last_checked)) {
            grow_terms(terms_len + 1);
            terms[terms_len++] = (seq_t)last_checked;
        }
    }
    return terms[n - 1];
}
