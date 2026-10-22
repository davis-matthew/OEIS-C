/*
 * A391264  a(3*n) = 1, a(3*n+1) = 3*n+1, a(3*n+2) = (3*n+1)*(3*n+2)/2.
 * https://oeis.org/A391264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3*n) = 1, a(3*n+1) = 3*n+1, a(3*n+2) = (3*n+1)*(3*n+2)/2
 * Range:     u8 n <= 25, u16 n <= 364, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // Determine n modulo 3
    idx_t mod = n % 3;
    if (mod == 0) {
        return (seq_t)1;
    } else if (mod == 1) {
        // a(3*k+1) = 3*k+1 = n
        return (seq_t)n;
    } else { /* mod == 2 */
        // a(3*k+2) = (3*k+1)*(3*k+2)/2 = (n-1)*n/2
        seq_t nm1 = (seq_t)(n - 1);
        seq_t nn  = (seq_t)n;
        return (nm1 * nn) / 2;
    }
}
