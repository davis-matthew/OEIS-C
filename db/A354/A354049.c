/*
 * A354049  The smallest number that includes all the digits of n but does not
 *          equal n.
 * https://oeis.org/A354049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 9998, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   extract_digits, sort_ascending, value_from_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the smallest positive integer that contains every digit of n
 * (at least as many times as in n) but is not equal to n, and does not start
 * with 0. The search is direct: try all permutations of n's digits (those that
 * form a legal number and differ from n) and, if necessary, add an extra digit
 * (0-9) and arrange digits to obtain the minimal value.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/extract_digits.h"
#include "../utils/sort_ascending.h"
#include "../utils/value_from_digits.h"

/* --------- helpers --------- */

/* lexicographic next_permutation; returns true if a new permutation was produced */
static bool next_permutation(int *a, size_t n) {
    if (n <= 1) return false;
    size_t i = n - 1;
    while (i > 0 && a[i - 1] >= a[i]) --i;
    if (i == 0) return false;
    size_t j = n;
    while (a[j - 1] <= a[i - 1]) --j;
    int tmp = a[i - 1];
    a[i - 1] = a[j - 1];
    a[j - 1] = tmp;
    for (size_t l = i, r = n - 1; l < r; ++l, --r) {
        tmp = a[l];
        a[l] = a[r];
        a[r] = tmp;
    }
    return true;
}

/* --------- main sequence function --------- */
seq_t a(idx_t n) {
    /* step 1: extract and sort digits of n */
    int digits[32];
    size_t len;
    extract_digits(n, digits, &len);
    sort_ascending(digits, len);

    /* compute min candidate via permutations of existing digits */
    seq_t best = (seq_t)-1;          /* max of unsigned type */
    bool found = false;
    int perm[32];
    for (size_t i = 0; i < len; ++i) perm[i] = digits[i];

    /* try all permutations */
    do {
        if (perm[0] == 0) continue;   /* leading zero not allowed */
        seq_t val = value_from_digits(perm, len);
        if (val == (seq_t)n) continue; /* must be different */
        if (!found || val < best) {
            best = val;
            found = true;
            if (best < (seq_t)n) break;   /* cannot get smaller than this */
        }
    } while (next_permutation(perm, len));

    if (found) return best;  /* a(n) found using same digits */

    /* step 2: add one extra digit (0-9) and find minimal arrangement */
    unsigned int freq[10] = {0};
    for (size_t i = 0; i < len; ++i) ++freq[digits[i]];

    best = (seq_t)-1;
    for (int dadd = 0; dadd <= 9; ++dadd) {
        unsigned int f2[10];
        for (int k = 0; k < 10; ++k) f2[k] = freq[k];
        ++f2[dadd];                     /* include the new digit */

        /* find the smallest non‑zero leading digit */
        int lead = -1;
        for (int k = 1; k <= 9; ++k) {
            if (f2[k] > 0) { lead = k; break; }
        }
        if (lead == -1) continue;       /* all digits zero – cannot form positive number */

        seq_t val = (seq_t)lead;
        --f2[lead];

        /* append remaining digits in ascending order */
        for (int k = 0; k <= 9; ++k) {
            while (f2[k] > 0) {
                val = val * (seq_t)10 + (seq_t)k;
                --f2[k];
            }
        }
        if (!found || val < best) {
            best = val;
            found = true;
        }
    }

    return best;   /* best must have been set by the previous step */
}
