/*
 * A250120  Coordination sequence for planar net 3.3.3.3.6 (also called the
 *          fsz net).
 * https://oeis.org/A250120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 5 + Σ_{i=0}^{n-2} p[i], where p = {4,6,4,5,5} repeated,
 *            a(0) = 1, a(1) = 5. This implements the known recurrence of OEIS
 *            A250120
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint8_t _pat[5] = {4, 6, 4, 5, 5};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    /* For n >= 2:
       count = n-1 is the number of terms of _pat summed.
       We split into full 5‑cycles and a remainder.           */
    idx_t count = n - 1;                /* number of pattern terms to sum */
    idx_t cycles = count / 5;
    idx_t rem    = count % 5;

    /* Sum over full cycles */
    seq_t sum = (seq_t)cycles * (seq_t)24;    /* each cycle sums to 24 */

    /* Add remainder terms */
    for (idx_t i = 0; i < rem; ++i)
        sum += (seq_t)_pat[i];

    /* Initial 5 accounts for a(0)=1 and d(1)=4 (1+4=5) */
    return (seq_t)5 + sum;
}
