/*
 * A338896  Inradii of Pythagorean triples of A338895.
 * https://oeis.org/A338896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2 * p * (k + 1 - p) where k = ⌊(√(8*n+1)-1)/2⌋ (largest
 *            integer with k(k+1)/2 ≤ n), t_k = k(k+1)/2, p = n - t_k. For
 *            triangular n (p = 0) the value is 0. The algorithm uses integer
 *            binary search to find k, avoiding floating point
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* sequence defined for n ≥ 1; return 0 for safety */
        return 0;

    /* Find k such that k*(k+1)/2 ≤ n < (k+1)*(k+2)/2 */
    idx_t low = 1, high = n, k = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n) {
            k = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    /* t_k = k*(k+1)/2 */
    idx_t t_k = ( (__int128)k * (k + 1) ) / 2;

    /* p = n - t_k  (0 ≤ p ≤ k) */
    idx_t p = n - t_k;

    /* a(n) = 2 * p * (k + 1 - p)   (zero when p = 0) */
    seq_t val = (seq_t)2 * (seq_t)p * (seq_t)(k + 1 - p);
    return val;
}
