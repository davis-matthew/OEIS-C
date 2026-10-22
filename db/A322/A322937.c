/*
 * A322937  Triangular array in which the n-th row lists the primes strongly
 *          prime to n (in ascending order). For the empty rows n = 1, 2, 3, 4
 *          and 6 we set by convention 0.
 * https://oeis.org/A322937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th term of the row‑wise flattening of the set * of
 *            primes strongly prime to each row index. Rows that contain no *
 *            such primes contribute a single 0
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"

/* Prime list cached between calls.  The list is grown as needed. */
static idx_t *g_prime_list = NULL;
static size_t g_prime_cnt = 0;
static idx_t g_prime_limit = 0;

/* Ensure that all primes up to 'limit' are available in g_prime_list. */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= g_prime_limit) return;
    idx_t *new_list = NULL;
    size_t new_cnt;
    build_primes(limit, &new_list, &new_cnt);
    free(g_prime_list);
    g_prime_list = new_list;
    g_prime_cnt = new_cnt;
    g_prime_limit = limit;
}

seq_t a(idx_t n)
{
    idx_t cumulative = 0;
    idx_t row = 1;

    while (cumulative < n) {
        ensure_primes_up_to(row);
        bool did_find = false;

        /* Scan every prime <= row to find strong primes of this row. */
        for (size_t i = 0; i < g_prime_cnt && g_prime_list[i] <= row; ++i) {
            idx_t p = g_prime_list[i];
            if (row % p != 0 && (row - 1) % p != 0) {
                did_find = true;
                ++cumulative;
                if (cumulative == n)
                    return (seq_t)p;
            }
        }

        /* If the row had no strong primes, we output a single 0. */
        if (!did_find) {
            ++cumulative;
            if (cumulative == n)
                return (seq_t)0;
        }

        ++row;
    }

    /* Should never happen for n >= 1 but keep compiler happy. */
    return (seq_t)0;
}
