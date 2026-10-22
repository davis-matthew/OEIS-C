/*
 * A391574  Numbers k such that 3*k = A048720(3,m) for some m, where A048720
 *          is carryless base-2 multiplication.
 * https://oeis.org/A391574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A391574: numbers k such that 3*k = A048720(3,m) for some m,
 * i.e. the popcount of 3*k is even.  The function returns the n‑th such k
 * (1‑based indexing). */
seq_t a(idx_t n)
{
    idx_t found = 0;            /* how many qualifying numbers we have seen */
    for (idx_t k = 1; ; ++k) {  /* iterate over all positive integers        */
        /* compute 3 * k as a seq_t value */
        seq_t x = (seq_t)k * 3;

        /* determine parity of popcount(x) */
        seq_t parity = 0;
        seq_t t = x;
        while (t) {
            parity ^= (t & 1);   /* flip when we see a 1 bit */
            t >>= 1;
        }

        /* if popcount(x) is even, k qualifies */
        if (parity == 0) {
            ++found;
            if (found == n) {
                return (seq_t)k;
            }
        }
    }
}
