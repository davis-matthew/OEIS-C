/*
 * A342502  Gaps between first elements of prime quintuples of the form (p,
 *          p+2, p+6, p+12, p+14). The quintuples are abutting:
 *          twin/cousin/sexy/twin pairs.
 * https://oeis.org/A342502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 25, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n), the gap between the first elements of
   successive prime quintuples of the form (p, p+2, p+6, p+12, p+14).
   The algorithm iterates over numbers p ≡ 17 (mod 30), since all
   quintuples beyond the first must begin with a prime congruent to
   17 modulo 30. For each candidate p it tests whether the five
   numbers are all prime using the supplied is_prime() helper.
   It stops when the (n+1)-st quintuple is found and returns the
   difference between the first elements of the (n+1)-st and n-th
   quintuples. */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Handle the offset: a(1) corresponds to difference between
       the 2nd and 1st quintuples. */
    uint64_t p = 17;           /* first candidate after the quintuple starting at 5 */
    uint64_t prev = 5;         /* first quintuple's starting prime */
    uint64_t count = 1;        /* number of quintuples found so far */

    while (true) {
        /* check if p is prime before testing the other offsets */
        if (is_prime(p) &&
            is_prime(p + 2) &&
            is_prime(p + 6) &&
            is_prime(p + 12) &&
            is_prime(p + 14)) {

            count++;                      /* new quintuple found */
            if (count == n + 1) {         /* we have found the (n+1)-th quintuple */
                return (seq_t)(p - prev);
            }
            prev = p;                     /* update previous start */
        }
        p += 30;                          /* next candidate with residue 17 mod 30 */
    }
}
