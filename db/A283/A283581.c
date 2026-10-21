/*
 * A283581  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 646", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A283581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is defined by the linear recurrence a(0..11) given, and for
 *            n > 11 a(n) = a(n-2) + 100000000*a(n-8) - 100000000*a(n-10)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies: none beyond the typenames seq_t and idx_t, which are
   supplied by the build system.  The routine uses only integer
   arithmetic and no dynamic allocation; it works for 8,16,32,64 or
   128‑bit unsigned integers and for an arbitrary‑precision integer
   class used in the C++ build. */

#include <stddef.h>
#include <stdbool.h>

static const seq_t base_vals[12] = {
    /* a(0)..a(11) */
    1ULL,
    11ULL,
    101ULL,
    1101ULL,
    10101ULL,
    110101ULL,
    1010001ULL,
    11011011ULL,
    101000001ULL,
    1101100011ULL,
    10101000101ULL,
    110101101101ULL
};

seq_t a(idx_t n)
{
    if (n < 12u) {
        return base_vals[n];
    }

    /* Buffer of the last 12 terms; we keep the value that belongs to
       the current index modulo 12.  All indices we need are within
       this window. */
    seq_t buf[12];
    for (idx_t i = 0; i < 12u; ++i) {
        buf[i] = base_vals[i];
    }

    const seq_t C = (seq_t)100000000ULL;   /* 10^8, cast once */

    seq_t val;
    for (idx_t i = 12u; i <= n; ++i) {
        /* a(i) = a(i-2) + C*a(i-8) - C*a(i-10) */
        val = buf[(i - 2u) % 12u]
            + C * buf[(i - 8u) % 12u]
            - C * buf[(i - 10u) % 12u];
        buf[i % 12u] = val;
    }

    return buf[n % 12u];
}
