/*
 * A282904  Concatenation of the numbers of elements of P{1}, P{1, 2}, P{1, 2,
 *          3}, ..., P{1, 2, 3, ..., n}; where P{A} denotes the power set of
 *          set A ordered by the size of the subsets, and in each subset,
 *          following the increasing order.
 * https://oeis.org/A282904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth digit of the concatenation of the decimal
 *            representations of the subset sizes of the power sets P{1, …, k}
 *            (excluding the empty set), where the subsets are ordered first
 *            by size and then lexicographically; the concatenation is
 *            performed for k = 1, 2, … . The sequence is obtained by run...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   digits_of, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digits_of.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Protect against a(n) being called with index 0 */
    if (n == 0)
        return 0;

    seq_t target = (seq_t)n;          /* 1‑based digit position we want */
    seq_t cum     = 0;                /* cumulative digits processed */

    /* Find the block k containing the target digit */
    for (idx_t k = 1; ; ++k) {
        seq_t binom = 1;              /* binomial coefficient C(k,0) */
        seq_t block = 0;              /* digits contributed by all subsets of size 1…k */

        /* Compute C(k,m) for m = 1…k and accumulate block length */
        for (idx_t m = 1; m <= k; ++m) {
            binom = binom * (k - m + 1) / m;          /* C(k,m) */
            int dlen = digits_of(m);                  /* decimal length of m */
            block += binom * (seq_t)dlen;
        }

        cum += block;                                    /* total digits up to this k */
        if (cum >= target) {                            /* target lies in this block */

            /* Position of the target digit within this block (0‑based) */
            seq_t pos = target - (cum - block) - 1;

            /* Find the subset size m that contains this digit */
            seq_t remaining = pos;
            binom = 1;                                   /* reset binom for second loop */
            for (idx_t m = 1; m <= k; ++m) {
                binom = binom * (k - m + 1) / m;          /* C(k,m) */
                int dlen = digits_of(m);
                seq_t group = binom * (seq_t)dlen;
                if (remaining < group) {                 /* digit is in this group */

                    /* Index of the digit within the representation of m */
                    idx_t idx_in_group = (idx_t)(remaining % (seq_t)dlen);

                    /* Compute the digit: most significant first */
                    idx_t exp = (idx_t)dlen - 1 - idx_in_group;
                    seq_t powv = pow10((idx_t)exp);
                    seq_t digit = ((seq_t)m / powv) % 10;
                    return digit;                        /* finished */
                }
                remaining -= group;
            }
        }
    }
}
