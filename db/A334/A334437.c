/*
 * A334437  Heinz number of the n-th reversed integer partition in graded
 *          lexicographical order.
 * https://oeis.org/A334437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 683, u32 n <= 2713, u64 n <= 2713,
 *            u128 n <= 2713, big any n
 * Helpers:   get_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = Heinz number of the n-th reversed integer partition in graded lexicographic order. */
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/seq_mul.h"

/* Recursive helper to generate all weakly increasing partitions of a given sum. */
static void gen_partitions(idx_t sum, idx_t min_part, seq_t current,
                           idx_t *cnt, idx_t n, seq_t *res, bool *found)
{
    if (*found) return;
    for (idx_t part = min_part; part <= sum; ++part) {
        seq_t prod = seq_mul(current, (seq_t)get_prime(part));
        if (part == sum) {
            ++(*cnt);
            if (*cnt == n) {
                *res = prod;
                *found = true;
                return;
            }
        } else {
            gen_partitions(sum - part, part, prod, cnt, n, res, found);
            if (*found) return;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;         /* empty partition */
    idx_t counter  = 0;
    seq_t result   = 0;
    bool found     = false;

    /* Enumerate partitions by increasing total sum. */
    for (idx_t total = 1; !found; ++total) {
        gen_partitions(total, 1, (seq_t)1, &counter, n, &result, &found);
    }
    return result;
}
