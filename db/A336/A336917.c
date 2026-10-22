/*
 * A336917  Number of iterations of x -> A252463(x) needed before the result
 *          is deficient, when starting from x=n.
 * https://oeis.org/A336917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 971, u16 n <= 9814, u32 n <= 9814, u64 n <= 9814,
 *            u128 n <= 9814, big any n
 * Helpers:   odd_map, sigma_of_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A336917: iterate x → A252463(x) (x even → x/2, x odd → product+sum of
 * distinct prime factors) until the result is deficient (σ(x) < 2x).  Return the
 * number of iterations performed. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/odd_map.h"
#include "../utils/sigma_of_val.h"

/* Return true if n is non‑deficient: σ(n) ≥ 2n. */
static inline bool is_nondeficient(idx_t n)
{
    return sigma_of_val(n) >= (seq_t)2 * n;
}

/* Main sequence function.  Count iterations until a deficient number is
 * produced. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t cur   = n;

    while (is_nondeficient(cur)) {
        /* map according to parity of cur */
        if ((cur & 1ULL) == 0)        /* even case: n/2 */
            cur >>= 1;
        else                          /* odd case: product+sum of distinct primes */
            cur = odd_map(cur);
        ++count;
    }
    return count;
}
