/*
 * A274134  Primes p such that both ror(p) and rol(p) are also primes, where
 *          ror(x)=A038572(x) is x rotated one binary place to the right,
 *          rol(x)=A006257(x) is x rotated one binary place to the left.
 * https://oeis.org/A274134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A274134: primes p for which both one‑place binary rotations of p are prime.
   a(n) = the n‑th such prime (offset 1).  The algorithm simply scans
   ascending candidates, tests primality with is_prime_big, computes
   right/left rotations, and counts qualifying primes until the
   requested index is reached.  The code is valid in C11 and C++17
   and works for any seq_t / idx_t provided by the build. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Count the number of bits needed to represent x (x > 0).  Works for
   arbitrary‑precision seq_t types because the shift operators are
   defined for seq_t. */
static unsigned bitlen_seq_t(seq_t x)
{
    unsigned len = 0;
    while (x) {
        x >>= 1;
        ++len;
    }
    return len;
}

/* Return the n‑th element of sequence A274134. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                                 /* offset 1 */

    idx_t count = 0;                                      /* found count */
    seq_t candidate = 3;                                  /* first odd >2 */

    while (1) {
        if (is_prime_big(candidate)) {
            unsigned k = bitlen_seq_t(candidate);

            /* right rotation */
            seq_t ror = (candidate >> 1) | ((candidate & 1) << (k - 1));

            /* left rotation */
            seq_t mask = ((seq_t)1 << k) - 1;
            seq_t rol  = ((candidate << 1) | (candidate >> (k - 1))) & mask;

            if (is_prime_big(ror) && is_prime_big(rol)) {
                ++count;
                if (count == n) {
                    return candidate;
                }
            }
        }

        /* next odd candidate */
        candidate += 2;

        /* protect against unsigned wrap‑around on tiny seq_t types */
        if (candidate < 3) break;
    }

    /* Should never reach here for n >= 1.  Return 0 as a fallback. */
    return 0;
}
