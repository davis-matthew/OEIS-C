/*
 * A347286  a(n) is n minus the number of odd divisors of n.
 * https://oeis.org/A347286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - A001227(n) // number of odd divisors of n subtracted
 *            from n
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n) {
    /* Strip all factors of 2 to obtain the odd part of n */
    idx_t odd_part = n;
    while ((odd_part & 1ULL) == 0) {
        odd_part >>= 1;
    }

    /* All divisors of the odd part are odd */
    uint64_t odd_div_cnt = divisor_count_u64(odd_part);

    /* a(n) = n - odd_div_cnt */
    return (seq_t)n - (seq_t)odd_div_cnt;
}
