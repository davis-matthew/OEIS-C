/*
 * A253436  Number of (2+1) X (n+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=39, a(2)=58, a(3)=70. For n >= 4, a(n) = 9 * 2^(n-1) + 30.
 *            (Derived from the recurrence a(n) = 3*a(n-1) - 2*a(n-2) for n >
 *            5.)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
    case 1: return (seq_t)39;
    case 2: return (seq_t)58;
    case 3: return (seq_t)70;
    default:
        /* Compute 9 * 2^(n-1) + 30.
           For integer types larger than 64 bits, shifting works as expected.
           For 64‑bit builds, n will never cause a shift beyond 63 bits in
           the tests (the values would overflow otherwise). */
        seq_t val = ((seq_t)1 << (n - 1));
        val = val * (seq_t)9 + (seq_t)30;
        return val;
    }
}
