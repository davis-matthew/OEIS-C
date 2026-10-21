/*
 * A293614  a(n) = (8*n + 18)*Pochhammer(n, 6) / 6!.
 * https://oeis.org/A293614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 42, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (8*n + 18) * binom(n + 5, 6)
 * where binom(n + 5, 6) = (n+5)! / (6! * n!)
 * This yields the sequence A293614 (offset 0).
 */

#include <stdint.h>
#include <stddef.h>

/* seq_t and idx_t are defined by the build system.
 * We produce the function required by the template.
 */
seq_t a(idx_t n)
{
    /* use a 128‑bit intermediate to avoid overflow during
     * intermediate multiplications, then cast the result to seq_t. */
    unsigned __int128 res = 1;

    /* compute binom(n + 5, 6) = product_{k=1..6} (n + 6 - k) / k */
    for (idx_t i = 1; i <= 6; ++i) {
        res = res * (unsigned __int128)(n + 6 - i) / i;
    }

    /* multiply by (8*n + 18) */
    res = res * ((unsigned __int128)8 * n + 18);

    return (seq_t)res;
}
