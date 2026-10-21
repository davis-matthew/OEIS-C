/*
 * A294057  Number of set partitions of [n] such that the maximal absolute
 *          difference between consecutive elements within a block equals
 *          eight.
 * https://oeis.org/A294057
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = f8(n) - f7(n) where f_k(n) is the number of set
 *            partitions of [n] such that the maximal absolute difference
 *            between consecutive elements within any block does not exceed k.
 *            For this sequence k=8 and k=7
 * Range:     u8 none, u16 n <= 10, u32 n <= 16, u64 n <= 28, u128 n <= 53,
 *            big any n
 * Helpers:   count_partitions_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required headers for types used in this file */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/count_partitions_upto.h"

/* Forward declaration of helper function that counts partitions
   with maximal gap <= max_gap.  No external utilities are needed. */
static seq_t count_partitions_upto(idx_t n, idx_t max_gap);

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 9.  For smaller n we return 0. */
    if (n < 9) {
        return (seq_t)0;
    }

    seq_t f8 = count_partitions_upto(n, (idx_t)8);
    seq_t f7 = count_partitions_upto(n, (idx_t)7);

    /* f8 and f7 will always satisfy f8 >= f7 for n >= 9, so subtraction
       is safe. */
    return f8 - f7;
}

/* --------------------------------------------------------------- */
/* Count partitions of the set {1,2,…,n} where every block satisfies
   the constraint that for each block, the difference between two
   consecutive elements (in increasing order) is at most max_gap. */
