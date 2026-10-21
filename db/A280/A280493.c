/*
 * A280493  Sum of GF(2)[X] divisors of n: the sum is ordinary sum of
 *          integers, the summands being all the natural numbers whose binary
 *          expansions encode such (0,1)-polynomials that divide the
 *          (0,1)-polynomial encoded by n when the polynomial factorization is
 *          done over the field GF(2).
 * https://oeis.org/A280493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m |_{GF(2)} n} m, summed over all 1 ≤ m ≤ n for which
 *            the polynomial encoded by m divides the polynomial encoded by n
 *            when working over GF(2). Here “|_{GF(2)}” means divisibility of
 *            the corresponding binary polynomials
 * Range:     u8 n <= 67, u16 n <= 2048, u32 n <= 2048, u64 n <= 2048,
 *            u128 n <= 2048, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return floor(log2(x)) for x > 0.  The result is the position of
   the most significant set bit (0‑based).  This implementation
   uses a simple loop, which is sufficient for the small degrees
   involved (≤ 63). */
static inline unsigned int log2_floor(uint64_t x)
{
    unsigned int r = 0;
    /* x > 0 */
    while ((x >>= 1) != 0)
        ++r;
    return r;
}

/* Test whether polynomial a divides polynomial b over GF(2).
   Both a and b are represented by the integer values of their
   binary coefficient strings.  Division is performed by the
   standard long‑division algorithm using XOR for addition. */
static bool gf2_divides(uint64_t a, uint64_t b)
{
    if (a == 0)          /* 0 never divides a non‑zero polynomial */
        return false;
    uint64_t rem = b;
    unsigned int deg_a = log2_floor(a);
    /* Fast‑path for the trivial divisor 1 */
    if (a == 1)
        return true;

    while (rem != 0) {
        unsigned int deg_rem = log2_floor(rem);
        if (deg_rem < deg_a)
            return false;
        unsigned int shift = deg_rem - deg_a;
        rem ^= a << shift;
    }
    return true;
}

/* Main sequence function.  It iterates over all 1 ≤ d ≤ n and
   adds d to the sum whenever the polynomial of d divides the
   polynomial of n.  All computations are done with 64‑bit
   integers, which are sufficient for the degree of the polynomials
   given that n is an idx_t (uint64_t).  The result is stored in
   seq_t, which may be a larger integer type or an arbitrary‑precision
   class; the cast (seq_t)d ensures that the addition uses the
   correct type. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for 0, but safe fallback */

    seq_t sum = 0;
    uint64_t nn = (uint64_t)n;      /* polynomial of n */

    for (idx_t d = 1; d <= n; ++d) {
        if (gf2_divides((uint64_t)d, nn))
            sum += (seq_t)d;
    }
    return sum;
}
