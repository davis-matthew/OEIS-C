/*
 * A352751  Modified Sisyphus function of order 4: a(n) is the concatenation
 *          of (number of digits of n)(number digits of n congruent to 0
 *          modulo 4)(number of digits of n congruent to 1 modulo 4)(number of
 *          digits of n congruent to 2 modulo 4)(number of digits of n
 *          congruent to 3 modulo 4).
 * https://oeis.org/A352751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation of ( number of digits of n, number of
 *            digits of n ≡0 (mod 4), number of digits of n ≡1 (mod 4), number
 *            of digits of n ≡2 (mod 4), number of digits of n ≡3 (mod 4))
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Counters for total digits and each residue class modulo 4 */
    unsigned total = 0, c0 = 0, c1 = 0, c2 = 0, c3 = 0;

    /* Handle the special case n == 0 (decimal digit '0') */
    if (n == 0) {
        total = 1;
        c0 = 1;               /* 0 ≡ 0 (mod 4) */
    } else {
        /* Count digits of n */
        while (n > 0) {
            uint64_t d = n % 10;
            n /= 10;
            ++total;
            switch ((unsigned)d % 4) {
                case 0: ++c0; break;
                case 1: ++c1; break;
                case 2: ++c2; break;
                default: ++c3; break;   /* 3 */
            }
        }
    }

    /* Convert each count to its decimal representation and concatenate */
    seq_t res = 0;
    /* Helper macro to multiply by 10 or 100 depending on number of digits */
    #define SHIFT_AND_ADD(cnt) do {                  \
            seq_t p = (cnt) < 10 ? 10 : 100;          \
            res = res * p + (cnt);                    \
        } while (0)

    SHIFT_AND_ADD(total);
    SHIFT_AND_ADD(c0);
    SHIFT_AND_ADD(c1);
    SHIFT_AND_ADD(c2);
    SHIFT_AND_ADD(c3);

    #undef SHIFT_AND_ADD
    return res;
}
