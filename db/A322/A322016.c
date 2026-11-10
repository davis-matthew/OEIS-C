/*
 * A322016  a(0) = 0; for n > 0, if A003188(n) > A003188(n-1) then a(n) = n-1,
 *          otherwise a(n) = 0.
 * https://oeis.org/A322016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n > 0, a(n) = n-1 if A003188(n) > A003188(n-1),
 *            else 0; A003188(n) = n XOR floor(n/2)
 * Range:     u8 n <= 255, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 115 terms through n = 9433 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A003188_a(idx_t n)
{
	seq_t nval = (seq_t)n;
	return nval ^ (nval >> 1);
}

seq_t a(idx_t n)
{
	if (n == 0)
		return (seq_t)0;
	if (A003188_a(n) > A003188_a(n - 1))
		return (seq_t)(n - 1);
	return (seq_t)0;
}
