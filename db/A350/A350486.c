/*
 * A350486  Numbers that have an equal number of even- and odd-length
 *          unordered factorizations and also an equal number of even- and
 *          odd-length unordered factorizations into distinct factors.
 * https://oeis.org/A350486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static void dfs_distinct(uint64_t n, uint64_t min_factor,
                         unsigned parity,
                         uint64_t *even, uint64_t *odd)
{
    if (parity & 1) ++*odd;
    else          ++*even;

    for (uint64_t d = min_factor; d <= n / d; ++d) {
        if (n % d == 0) {
            uint64_t m = n / d;
            if (d < m)          /* guarantee distinctness */
                dfs_distinct(m, d + 1, parity + 1, even, odd);
        }
    }
}

static void dfs_unrestricted(uint64_t n, uint64_t min_factor,
                             unsigned parity,
                             uint64_t *even, uint64_t *odd)
{
    /* single‑factor factorisation (n itself) */
    if (parity & 1) ++*odd;
    else          ++*even;

    for (uint64_t d = min_factor; d <= n / d; ++d) {
        if (n % d == 0) {
            uint64_t m = n / d;
            dfs_unrestricted(m, d, parity + 1, even, odd);
        }
    }
}

/* Find the n‑th integer k>1 such that:
 *   • the number of unordered factorisations of k (repetitions allowed)
 *     has equal numbers of even‑ and odd‑length factorizations;
 *   • the number of unordered factorisations of k into distinct factors
 *     also has equal numbers of even‑ and odd‑length factorizations. */

static void push_value(seq_t **array, size_t *len, size_t *cap, seq_t value)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *array, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);    /* allocation failure */
        }
        *array = tmp;
        *cap = newcap;
    }
    (*array)[*len] = value;
    ++(*len);
}

seq_t a(idx_t n)
{
    static seq_t *values = NULL;
    static idx_t count = 0;       /* number of terms generated so far */
    static size_t cap = 0;        /* capacity of the array */
    static uint64_t last_candidate = 1;  /* largest tested number */

    /* Generate terms until we have count >= n */
    while (count < n) {
        ++last_candidate;
        uint64_t e_u = 0, o_u = 0;
        dfs_unrestricted(last_candidate, 2, 1, &e_u, &o_u);

        uint64_t e_d = 0, o_d = 0;
        dfs_distinct(last_candidate, 2, 1, &e_d, &o_d);

        if (e_u == o_u && e_d == o_d) {
            push_value(&values, &count, &cap, (seq_t)last_candidate);
        }
    }

    return values[n - 1];    /* 1‑based index */
}
