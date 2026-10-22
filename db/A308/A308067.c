/*
 * A308067  Number of integer-sided triangles with perimeter n whose longest
 *          side length is odd.
 * https://oeis.org/A308067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integer‑sided triangles with perimeter n whose
 *            longest side length is odd
 * Range:     u8 n <= 150, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 3) return 0;                 /* no triangle possible */

    /* c is the longest side; it must be odd, satisfy c < n/2
       and be at least ceil(n/3) (otherwise a <= b <= c impossible) */
    idx_t c_min = (n + 2) / 3;            /* ceil(n/3) */
    idx_t c_max = (n - 1) / 2;            /* floor((n-1)/2) */
    if (c_min > c_max) return 0;

    seq_t result = 0;
    idx_t c_start = c_min;
    if ((c_start & 1) == 0) c_start++;   /* make it odd */

    for (idx_t c = c_start; c <= c_max; c += 2) {
        idx_t S = n - c;                   /* a + b */
        idx_t L = n - 2 * c;               /* lower bound for a   */
        idx_t U = S / 2;                   /* upper bound for a   */
        if (U >= L) {
            idx_t subcnt = U - L + 1;
            result += (seq_t)subcnt;
        }
    }
    return result;
}
