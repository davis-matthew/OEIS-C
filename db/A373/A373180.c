/*
 * A373180  a(n) = min{k : KroneckerSymbol(n, k) = 1} if n > 1 and k > 1, a(0)
 *          = 1, a(1) = 0.
 * https://oeis.org/A373180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k>1 : KroneckerSymbol(n,k)=1} for n>1, with a(0)=1,
 *            a(1)=0
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 86 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the Kronecker symbol (a|n) where a>0 and n>0.
   Returns 1, 0, or -1. */
static int kronecker_uint64(uint64_t a, uint64_t n)
{
    int res = 1;

    /* factor powers of 2 from the modulus n */
    while ((n & 1) == 0) {
        n >>= 1;
        if ((a & 1) == 0) {          /* a even, symbol is zero */
            return 0;
        }
        uint8_t a8 = a & 7;
        if (a8 == 1 || a8 == 7) {
            /* res *= 1; nothing to do */
        } else if (a8 == 3 || a8 == 5) {
            res = -res;
        } else {
            return 0;                /* should not occur */
        }
    }

    /* now n is odd - compute (a|n) via the Jacobi algorithm */
    while (a != 0) {
        /* peel off powers of 2 from a */
        while ((a & 1) == 0) {
            a >>= 1;
            uint8_t n8 = n & 7;
            if (n8 == 1 || n8 == 7) {
                /* res *= 1; */
            } else if (n8 == 3 || n8 == 5) {
                res = -res;
            } else {
                return 0;            /* should not occur – n odd */
            }
        }

        /* both a and n are odd – apply quadratic reciprocity */
        if ((a & 3) == 3 && (n & 3) == 3) {
            res = -res;
        }

        uint64_t temp = a;
        a = n % temp;
        n = temp;
    }

    /* if the algorithm ended with n==1 we have a proper symbol */
    return (n == 1) ? res : 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    uint64_t N = (uint64_t)n;
    uint64_t k = 2;

    for (;;) {
        if (kronecker_uint64(N, k) == 1) {
            return (seq_t)k;
        }
        ++k;
    }
}
