/*
 * A366471  Number of increasing geometric progressions in {1,2,3,...,n} with
 *          rational ratio.
 * https://oeis.org/A366471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + Σ_{k=2..1+⌊log₂ n⌋} Σ_{p=2..⌊n¹⁄(k-1)⌋} φ(p) · ⌊n /
 *            p^(k-1)⌋
 * Range:     u8 n <= 21, u16 n <= 359, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 710 (2026-11-01)
 * Helpers:   ilog2, ipow_u64_idx, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where φ is Euler’s totient function. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ilog2.h"
#include "../utils/ipow_u64_idx.h"
#include "../utils/totient_seq.h"

seq_t a(idx_t n)
{
    /* Count all singletons */
    seq_t result = (seq_t)n;

    /* For n < 2 we already have the correct answer */
    if (n < 2)
        return result;

    /* Maximum possible length of a GP in {1,…,n} is 1+⌊log₂ n⌋ */
    idx_t kmax = 1 + ilog2(n);

    for (idx_t k = 2; k <= kmax; ++k) {
        idx_t exp = k - 1;               /* exponent in p^(k-1)          */
        for (idx_t p = 2; ; ++p) {
            /* Compute p^(k-1); stop when the power exceeds n */
            seq_t power = ipow_u64_idx(p, exp);
            if (power > (seq_t)n)
                break;
            /* φ(p) is computed using the provided function  */
            seq_t phi = totient_seq((seq_t)p);
            /* floor(n / p^(k-1)) is an integer division of seq_t types */
            seq_t term = phi * (((seq_t)n) / power);
            result += term;
        }
    }
    return result;
}
