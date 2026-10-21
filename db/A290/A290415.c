/*
 * A290415  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 785", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0,1 a(2) = 5, a(3) = 7, a(4) = 19 a(n) =
 *            2^(n/2) - 1 for even n ≥ 6 a(n) = 2^(n + 1) - 1 for odd n ≥ 5
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t, if needed */

/* Helper: compute 2^exp as seq_t.  For exp < 64 we use a shift, otherwise
   we iteratively multiply by two to avoid undefined behaviour on unsigned
   shift amounts.  Works for arbitrary‑precision seq_t as well. */
static seq_t pow2(idx_t exp)
{
    if (exp < 64)
        return ((seq_t)1 << exp);
    else
    {
        seq_t res = 1;
        for (idx_t i = 0; i < exp; ++i)
            res *= 2;
        return res;
    }
}

/* External linkage function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 1;
    if (n == 2)
        return 5;
    if (n == 3)
        return 7;
    if (n == 4)
        return 19;

    if ((n & 1ULL) == 0ULL)          /* even */
        return pow2(n / 2) - 1;
    else                              /* odd */
        return pow2(n + 1) - 1;
}
