/*
 * A256432  Characteristic function of octahedral numbers.
 * https://oeis.org/A256432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is an octahedral number (2*m^3+m)/3 for some
 *            integer m, else 0
 * Range:     u8 n <= 42, u16 n <= 9879, u32 n <= 9879, u64 n <= 9879,
 *            u128 n <= 9879, big any n
 * Helpers:   _octahedral_value, floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_cube_root.h"
#include "../utils/_octahedral_value.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    /* First approximation of m = floor((3*N)/2)^(1/3). */
    seq_t approx = ((seq_t)3 * N) / 2;
    seq_t m     = floor_cube_root(approx);

    /* Check whether N is exactly (2*m^3 + m)/3. */
    if (_octahedral_value(m) == N)
        return 1;
    else
        return 0;
}
