/*
 * A347658  Number of partitions of n into at most 3 powers of 2.
 * https://oeis.org/A347658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count partitions of n into at most 3 powers of 2.
   The routine enumerates all unordered triples of powers of two whose
   sum is n, treating the cases of one and two parts separately. */
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;                    /* empty partition */

    seq_t count = 0;

    /* 1 part: n itself must be a power of two */
    if ( n > 0 && ((n & (n - 1)) == 0) )
        count += 1;

    /* 2 parts: find unordered pairs (q, p) with q <= p and q + p = n */
    for (idx_t e1 = 0; ; ++e1) {
        seq_t q = pow2(e1);
        if ( (idx_t)q > n ) break;
        seq_t p = (seq_t)n - q;
        if (p < q) continue;                /* enforce q <= p */
        if ( (p & (p - 1)) == 0 )            /* p is a power of two  */
            count += 1;
    }

    /* 3 parts: unordered triples p >= q >= r with p + q + r = n */
    for (idx_t er = 0; ; ++er) {
        seq_t r = pow2(er);
        if ( (idx_t)r > n ) break;
        for (idx_t eq = er; ; ++eq) {
            seq_t q = pow2(eq);
            seq_t sum_qr = r + q;
            if ( sum_qr > n ) break;
            seq_t p = (seq_t)n - sum_qr;     /* remaining part */
            if (p < q) continue;            /* enforce p >= q */
            if ( (p & (p - 1)) == 0 )        /* p is a power of 2   */
                count += 1;
        }
    }

    return count;
}
