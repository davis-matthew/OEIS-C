/*
 * A322015  If A003188(n+1) < A003188(n), then a(n) = n+1, otherwise a(n) = 0.
 * https://oeis.org/A322015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n+1 if A003188(n+1) < A003188(n), else 0; A003188(n) = n
 *            XOR floor(n/2)
 * Range:     u8 n <= 257, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 112 terms through n = 9552 (2026-11-08)
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
	seq_t g0 = A003188_a(n);
	seq_t g1 = A003188_a(n + 1);
	if (g1 < g0)
		return (seq_t)n + 1;
	return 0;
}
