/*
 * A371531  a(n) is the multiplicative order of A053669(n) modulo n.
 * https://oeis.org/A371531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For odd n: a(n) = multiplicative order of 2 modulo n. For even n
 *            (n = 2k): a(n) = multiplicative order of the smallest odd
 *            integer >1 that is coprime to k, modulo n. This implements the
 *            OEIS definition a(n) = ord(A053669(n), n) with A053669(n) = 2
 *            for odd n and A284723(k) (the smallest odd intege...
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   gcd, mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mod_mul.h"

static inline seq_t multiplicative_order(seq_t base, idx_t mod)
{
    seq_t pow = base % (seq_t)mod;
    seq_t e   = 1;
    /* Special case: if mod == 1 (i.e., n == 1), return 1 */
    if ((seq_t)mod == 1) return (seq_t)1;
    while (pow != (seq_t)1)
    {
        pow = (seq_t)mod_mul((uint64_t)pow, (uint64_t)base, (uint64_t)mod);
        ++e;
    }
    return e;
}

seq_t a(idx_t n)
{
    /* Handle the offset of the sequence */
    if (n == 1) return (seq_t)1;

    if (n & 1)            /* odd n: base 2 */
    {
        return multiplicative_order((seq_t)2, n);
    }
    else                  /* even n: n = 2*k */
    {
        idx_t k = n >> 1;      /* k = n/2 */

        /* Find the smallest odd integer >1 that is coprime to k */
        uint64_t candidate = 3;
        while (gcd((seq_t)candidate, (seq_t)k) != (seq_t)1)
            candidate += 2;

        return multiplicative_order((seq_t)candidate, n);
    }
}
