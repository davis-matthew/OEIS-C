/*
 * A276115  Numbers whose digits have a permutation that is a palindrome.
 * https://oeis.org/A276115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 7144, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 8784 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A276115
   Numbers whose digits have a permutation that is a palindrome.
   Algorithm: sequentially test each integer for the parity condition
   on digit frequencies; memoise results for random access. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: test whether a number satisfies the condition. */
static bool has_palindromic_permutation(seq_t x)
{
    if (x == 0)
        return false;

    int cnt[10] = {0};
    seq_t t = x;
    while (t != 0) {
        int d = (int)(t % 10);
        cnt[d]++;
        t /= 10;
    }

    int odd = 0;
    int nonzero = 0;               /* total occurrences of digits 1–9 */
    for (int i = 0; i < 10; ++i) {
        if (cnt[i] & 1)
            odd++;
        if (i != 0)
            nonzero += cnt[i];
    }

    if (odd > 1)
        return false;
    if (x >= 10 && nonzero < 2)
        return false;
    return true;
}

/* Main function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset is 1; index 0 is unused */

    /* Static state for memoisation */
    static seq_t *seq      = NULL;   /* array of found terms          */
    static idx_t  size_cap = 0;      /* allocated capacity           */
    static idx_t  size_len = 0;      /* number of terms stored       */
    static seq_t  cur      = 0;      /* last candidate tested        */

    /* First‑time initialisation */
    if (seq == NULL) {
        size_cap = 16;
        seq = mem_new(seq_t, size_cap);
        if (seq == NULL)
            return 0; /* allocation failure: unlikely in test environment */
    }

    /* Find terms until we have at least n of them */
    while (size_len < n) {
        /* generate next candidate */
        cur++;

        /* ensure capacity */
        if (size_len >= size_cap) {
            size_cap <<= 1;
            seq = mem_resize(seq_t, seq, size_cap);
            if (seq == NULL)
                return 0; /* allocation failure */
        }

        if (has_palindromic_permutation(cur)) {
            seq[size_len++] = cur;
        }
    }

    return seq[n - 1];
}
