/*
 * A391570  a(n) is the smallest odd k > 1 such that k*n = A048720(k,m) for
 *          some m, where A048720 is carryless base-2 multiplication.
 * https://oeis.org/A391570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1382, u16 n <= 9728, u32 n <= 9728, u64 n <= 9728,
 *            u128 n <= 9728, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A391570: a(n) is the smallest odd integer k>1 such that
   there exists an integer m with carryless base‑2 multiplication
   k ⊗ m = k*n.  This implementation checks odd k by polynomial
   division over GF(2).  All work is done with an unsigned
   __int128 working type to avoid overflows for small seq_t builds.  */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Return the number of bits needed to represent v (zero gives 0). */
static unsigned bitlen_uint128(unsigned __int128 v)
{
    unsigned len = 0;
    while (v) {
        ++len;
        v >>= 1;
    }
    return len;
}

/* Return true iff there exists m such that k ⊗ m = k*n.  k is odd >1. */
static bool admissible(idx_t n, idx_t k)
{
    /* Work in a type that can hold the product for all builds. */
    unsigned __int128 t = (unsigned __int128)n * (unsigned __int128)k;
    unsigned __int128 kk = (unsigned __int128)k;

    if (t == 0)            /* n>0 and k>1, so this never happens. */
        return true;

    unsigned deg_k = bitlen_uint128(kk);
    unsigned deg_t = bitlen_uint128(t);

    /* Polynomial long division over GF(2). */
    while (deg_t >= deg_k) {
        unsigned shift = deg_t - deg_k;
        t ^= (kk << shift);
        deg_t = bitlen_uint128(t);
    }
    return t == 0;         /* remainder zero ⇒ suitable m exists. */
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Search odd k starting from 3 until a solution is found. */
    for (idx_t k = 3; ; k += 2) {
        if (admissible(n, k))
            return (seq_t)k;
    }
}
