/*
 * A336007  Numbers whose mixed Zeckendorf-Lucas representation is not a
 *          Zeckendorf or Lucas representation. See Comments.
 * https://oeis.org/A336007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose greedy expansion using the union of Fibonacci and
 *            Lucas * numbers contains at least one Fibonacci term that is not
 *            a Lucas number * and at least one Lucas term that is not a
 *            Fibonacci number
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   extend_fib_upto, extend_luc_upto, largest_le
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/extend_fib_upto.h"
#include "../utils/extend_luc_upto.h"
#include "../utils/largest_le.h"

/* Helper: binary search for membership */
static bool in_array(const seq_t *arr, size_t len, seq_t x)
{
    for (size_t i = 0; i < len; ++i)
        if (arr[i] == x) return true;
    return false;
}

/* Determine if 'n' has a mixed representation that contains at least
 * one Fibonacci term not also Lucas and one Lucas term not also Fibonacci. */
static bool is_mixed(seq_t n)
{
    static seq_t fib[128];
    static size_t fib_len = 0;
    static seq_t luc[128];
    static size_t luc_len = 0;

    extend_fib_upto(fib, &fib_len, 128, n);
    extend_luc_upto(luc, &luc_len, 128, n);

    seq_t rem = n;
    bool has_excl_fib = false;
    bool has_excl_luc = false;

    while (rem > 0) {
        seq_t cand_fib = largest_le(fib, fib_len, rem);
        seq_t cand_luc = largest_le(luc, luc_len, rem);
        seq_t cand = (cand_fib > cand_luc) ? cand_fib : cand_luc;
        rem -= cand;

        bool is_fib = in_array(fib, fib_len, cand);
        bool is_luc = in_array(luc, luc_len, cand);

        if (is_fib && !is_luc) has_excl_fib = true;
        if (is_luc && !is_fib) has_excl_luc = true;
    }

    return has_excl_fib && has_excl_luc;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Undefined for index 0, but guard against it */

    idx_t count = 0;
    idx_t candidate = 1;

    while (1) {
        seq_t cand_seq = (seq_t)candidate;

        if (is_mixed(cand_seq)) {
            ++count;
            if (count == n) return cand_seq;
        }
        ++candidate;
    }
}
