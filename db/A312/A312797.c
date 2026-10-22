/*
 * A312797  Coordination sequence Gal.5.186.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordination sequence A312797 for Gal.5.186.2.
   Formula: for n < 6 use the first six terms.
   For n ≥ 6, let t = n / 6 and r = n % 6.
   Then:
          r = 0: a(n) = 28*t – 2
          r = 1: a(n) = 28*t + 6
          r = 2: a(n) = 8  + 32*t
          r = 3: a(n) = 16 + 28*t
          r = 4: a(n) = 20 + 32*t
          r = 5: a(n) = 26 + 28*t
   This reproduces the sequence 1,4,8,16,20,26,26,34,… */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t base[6] = {
        (seq_t)1,
        (seq_t)4,
        (seq_t)8,
        (seq_t)16,
        (seq_t)20,
        (seq_t)26
    };

    if (n < 6)
        return base[(size_t)n];

    idx_t t = n / 6;
    idx_t r = n % 6;

    switch ((unsigned int)r)
    {
        case 0: return (seq_t)(28 * t - 2);
        case 1: return (seq_t)(28 * t + 6);
        case 2: return (seq_t)(8 + 32 * t);
        case 3: return (seq_t)(16 + 28 * t);
        case 4: return (seq_t)(20 + 32 * t);
        case 5: return (seq_t)(26 + 28 * t);
    }

    return (seq_t)0;   /* unreachable */
}
