/*
 * A364360  a(n) = dpf(n) ^ tpf(n), where dpf(n) is the number of distinct
 *          prime factors of n if n >= 2 and otherwise = 0; tpf(n) is the
 *          number of all prime factors of n if n >= 2 and otherwise = 0.
 * https://oeis.org/A364360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = dpf(n)^tpf(n)
 * Range:     u8 n <= 209, u16 n <= 419, u32 n <= 419, u64 n <= 419,
 *            u128 n <= 419, big any n
 * Validated: standard, pass, 98 terms through n = 404 (2026-11-01)
 * Helpers:   bigomega_seq, ipow, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  dpf(n) = number of distinct prime factors of n (if n>=2, else 0)  */
/*  tpf(n) = total number of prime factors of n with multiplicity (if n>=2, else 0)  */

#include <stdint.h>                 /* for uint64_t */
#include <stdbool.h>
#include "../utils/bigomega_seq.h"
#include "../utils/ipow.h"
#include "../utils/omega.h"

seq_t a(idx_t n)
{
    /* OEIS offset 0, base cases */
    if (n < 2)                /* n = 0 or 1 -> a(n) = 1 */
        return (seq_t)1;

    /* number of distinct prime factors of n */
    seq_t d = (seq_t)omega((uint64_t)n);

    /* number of prime factors of n (with multiplicity) */
    seq_t t = bigomega_seq((seq_t)n);

    /* compute d ^ t */
    return ipow(d, t);
}
