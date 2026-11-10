/*
 * A322017  a(0) = 0; for n > 0, if A003188(n) > A003188(n-1) then a(n) = n-1,
 *          else if A003188(n+1) < A003188(n) then a(n) = n+1, otherwise a(n)
 *          = 0.
 * https://oeis.org/A322017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n > 0, g(n) = n XOR floor(n/2) (A003188); a(n) =
 *            n-1 if g(n) > g(n-1), else n+1 if g(n+1) < g(n), else 0
 * Range:     u8 n <= 256, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 107 terms through n = 9854 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A003188_g(idx_t n)
{
	return n ^ (n >> 1);
}

seq_t a(idx_t n)
{
	if (n == 0)
		return (seq_t)0;
	if (A003188_g(n) > A003188_g(n - 1))
		return (seq_t)(n - 1);
	if (A003188_g(n + 1) < A003188_g(n))
		return (seq_t)n + 1;
	return (seq_t)0;
}
