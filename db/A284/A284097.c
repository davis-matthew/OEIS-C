/*
 * A284097  a(n) = Sum_{d|n, d == 1 (mod 5)} d.
 * https://oeis.org/A284097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d == 1 (mod 5)} d
 * Range:     u8 n <= 215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    if (n == 0) return sum;      /* offset starts at 1; n>=1 normally */

    /* compute floor(sqrt(n)) without overflow */
    idx_t root = 0;
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {          /* avoid division by zero when n=0, not needed for n>=1 */
            low = 1;
            continue;
        }
        if (mid <= n / mid) {    /* mid^2 <= n */
            root = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    for (idx_t i = 1; i <= root; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            if (d1 % 5 == 1) {
                sum += (seq_t)d1;
            }
            if (d2 != d1 && d2 % 5 == 1) {
                sum += (seq_t)d2;
            }
        }
    }
    return sum;
}
