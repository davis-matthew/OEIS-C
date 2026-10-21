/*
 * A294754  Number of permutations of [n] avoiding {4321, 4123, 1234}.
 * https://oeis.org/A294754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   compute_all_counts, gen_perms_rec, patterns_present
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/compute_all_counts.h"
#include "../utils/gen_perms_rec.h"
#include "../utils/patterns_present.h"

/* This function counts permutations of [n] avoiding the patterns
   4321, 4123, 1234.  For n ≤ 9 it enumerates all permutations by
   recursion; for n ≥ 10 it returns 0 since any permutation of length
   10 or more necessarily contains an increasing or decreasing
   subsequence of length 4.  The enumeration is performed only once
   for all n ≤ 9 and the results are cached. */

seq_t a(idx_t n) {
    static bool table_ready = false;
    static seq_t table[10];
    if (n < 10) {
        if (!table_ready) {
            compute_all_counts(table);
            table_ready = true;
        }
        return table[n];
    }
    return 0;
}
