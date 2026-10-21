/*
 * A246444  Nonnegative integers k satisfying sin(k) > sec(k).
 * https://oeis.org/A246444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th positive integer k such that sin(k) > sec(k).
   The inequality is equivalent to k satisfying
   floor(226*k/355) % 4 == 1 or 2.  (226/355 ≈ 2/π, with error < 5.6×10⁻⁸,
   sufficient for k up to about 10⁵.  The loop therefore enumerates integers
   until the n‑th qualifying value is found.) */
seq_t a(idx_t n)
{
    idx_t k   = 1;          /* current integer to test */
    idx_t cnt = 0;          /* how many satisfying numbers found */

    while (1) {
        /* floor(226*k/355) – integer arithmetic, safe for k up to at least 10⁵ */
        idx_t m = (226 * k) / 355;
        idx_t r = m & 3;    /* m % 4 */

        if (r == 1 || r == 2) {
            ++cnt;
            if (cnt == n)
                return (seq_t)k;
        }
        ++k;
    }
}
