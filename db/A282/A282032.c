/*
 * A282032  Additive number system based on U.S. coins.
 * https://oeis.org/A282032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) for n > 11 Initial values: a(1)=1,
 *            a(2)=2, a(3)=3, a(4)=4, a(5)=5, a(6)=10, a(7)=15, a(8)=20,
 *            a(9)=25, a(10)=50, a(11)=100
 * Range:     u8 n <= 12, u16 n <= 665, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
/* For n ≥ 11 the closed form is a(n) = 100*(n-10). */
seq_t a(idx_t n)
{
    static const seq_t init[11] = {0,1,2,3,4,5,10,15,20,25,50};
    if (n <= 10)
        return init[(size_t)n];
    return (seq_t)(100ULL * (n - 10));
}
