/*
 * A381631  Numbers k such that the product of k and its digits is divisible
 *          by the sum of its digits.
 * https://oeis.org/A381631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   digit_sum, get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the nth integer k such that k · (product of its digits) is divisible
   by (sum of its digits).  The sequence is indexed in increasing order
   starting at a(1)=1. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/get_digits.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety: n ≥ 1 in the problem statement */

    idx_t found = 0;                 /* number of qualifying terms found so far   */
    seq_t k = 1;                     /* candidate integer k (as seq_t)             */

    /* Temporary buffer for decimal digits of k; 1024 digits is ample for all
       realistic n up to several thousand. */
    uint8_t buf[1024];
    size_t len;

    while (1) {
        /* Sum of digits of k */
        seq_t sum_seq = digit_sum(k);
        unsigned long long sum = (unsigned long long)sum_seq;  /* sum is always small */

        /* If the sum is 0 (possible only for k=0) skip (but k never 0 here). */
        if (sum == 0) {
            k++;
            continue;
        }

        /* Compute (product of digits of k) modulo sum. */
        unsigned long long prod_mod = 1 % sum; /* handle sum==1 correctly */
        get_digits(k, buf, &len);              /* buf now holds decimal digits of k */

        for (size_t i = 0; i < len; ++i) {
            unsigned long long d = (unsigned long long)buf[i];
            prod_mod = (prod_mod * d) % sum;
        }

        /* Compute k modulo sum and test divisibility condition. */
        seq_t kmod_seq = k % (seq_t)sum;
        unsigned long long kmod = (unsigned long long)kmod_seq;
        if ((kmod * prod_mod) % sum == 0) {
            ++found;
            if (found == n) return k;
        }

        ++k;          /* move to next integer */
    }
}
