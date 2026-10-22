/*
 * A363919  a(n) = n^excess(n), where excess(n) = A046660(n).
 * https://oeis.org/A363919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^A046660(n) = n^(Σ_{p|n} (v_p(n)-1))
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes excess = total multiplicity sum minus number of distinct primes      */
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* n is at least 1 (offset 1).  For n==1 the exponent is 0. */
    uint64_t tmp = n;
    int exp = 0;

    /* Factor out 2's */
    if ((tmp & 1ULL) == 0) {
        int m = 0;
        do { tmp >>= 1; m++; } while ((tmp & 1ULL) == 0);
        exp += m - 1;          /* m-1 for each 2-factor */
    }

    /* Odd primes */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int m = 0;
            do { tmp /= p; m++; } while (tmp % p == 0);
            exp += m - 1;
        }
    }

    /* Remaining prime > 1 has exponent 1, contributes 0 to excess */
    /* exp already contains the correct excess */
    return seq_pow((seq_t)n, (idx_t)exp);
}
