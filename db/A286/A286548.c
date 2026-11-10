/*
 * A286548  a(n) = A003188(n) - n.
 * https://oeis.org/A286548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = A003188(n) - n, where A003188(n) = n XOR floor(n/2)
 * Range:     i8 n <= 184, i16 n <= 16383, i32 n <= 16383, i64 n <= 16383,
 *            i128 n <= 16383, big any n
 * Validated: standard, pass, 98 terms through n = 9940 (2026-11-08)
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
	return A003188_a(n) - (seq_t)n;
}
