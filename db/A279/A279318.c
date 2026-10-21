/*
 * A279318  Permutation of the nonnegative integers [6k+3, 6k+2, 6k+1, 6k,
 *          6k+5, 6k+4].
 * https://oeis.org/A279318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*(n/6) + [3,2,1,0,5,4][n%6]
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   undefined for n < 0 (n is of type idx_t, unsigned) */

seq_t a(idx_t n)
{
    idx_t k = n / 6;
    idx_t r = n % 6;
    seq_t base = (seq_t)k * 6;

    switch (r) {
        case 0: return base + 3;
        case 1: return base + 2;
        case 2: return base + 1;
        case 3: return base + 0;
        case 4: return base + 5;
        case 5: return base + 4;
        default: return 0; /* unreachable */
    }
}
