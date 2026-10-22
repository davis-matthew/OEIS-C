/*
 * A398336  Numbers m coprime to 3 such that, for k >= 0, sopfr(3^k*m) <=
 *          sopfr(j) for j > 3^k*m, where sopfr is A001414; numbers dividing
 *          40 or 1024.
 * https://oeis.org/A398336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth smallest integer dividing 40 or 1024, equivalently
   the nth smallest m coprime to 3 satisfying sopfr(3ᵏm) ≤ sopfr(j) for all j > 3ᵏm. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Sorted list of divisors of 40 or 1024 (all coprime to 3). */
    static const seq_t terms[] = {
        (seq_t)1,  (seq_t)2,  (seq_t)4,  (seq_t)5,
        (seq_t)8,  (seq_t)10, (seq_t)16, (seq_t)20,
        (seq_t)32, (seq_t)40, (seq_t)64, (seq_t)128,
        (seq_t)256,(seq_t)512,(seq_t)1024
    };
    size_t len = sizeof(terms) / sizeof(terms[0]);

    /* OEIS offset is 1. */
    if (n == 0) return (seq_t)0;              /* safety */
    if ((idx_t)(n - 1) < len) return terms[n - 1];
    return (seq_t)0; /* undefined for n beyond the finite set. */
}
