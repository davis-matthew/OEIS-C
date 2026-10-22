/*
 * A396493  Number of satisfiable 3-SAT formulas with n variables and 4
 *          clauses in the multiset clause model.
 * https://oeis.org/A396493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = binomial(binomial(2*n+2,3)+3,4) - n*(16*n^6 + 48*n^5 +
 *            340*n^4 + 180*n^3 + 2818*n^2 - 10011*n + 6789)/18
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    // Compute B = binom(2*n+2, 3)
    idx_t two_n = 2 * n;
    idx_t B = ((idx_t)(2 * n + 2) * (idx_t)(2 * n + 1) * (idx_t)two_n) / 6;

    // Total number of 4–clause formulas: binom(B + 3, 4)
    seq_t total = binom(B + 3, 4);

    // Compute polynomial P(n)
    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;
    seq_t n3  = n2 * sn;
    seq_t n4  = n3 * sn;
    seq_t n5  = n4 * sn;
    seq_t n6  = n5 * sn;

    seq_t P =
        (seq_t)16   * n6 +
        (seq_t)48   * n5 +
        (seq_t)340  * n4 +
        (seq_t)180  * n3 +
        (seq_t)2818 * n2 -
        (seq_t)10011 * sn +
        (seq_t)6789;

    seq_t unsat_num = sn * P;
    seq_t unsat     = unsat_num / (seq_t)18;

    return total - unsat;
}
