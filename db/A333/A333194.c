/*
 * A333194  a(n) = Sum_{k=1..n} (ceiling(n/k) mod 2) * k.
 * https://oeis.org/A333194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (ceil(n/k) mod 2) * k
 * Range:     u8 n <= 48, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t ceil_val = (n - 1) / k + 1;   /* ceil(n/k) without overflow */
        if (ceil_val & 1) {                  /* odd? */
            sum += (seq_t)k;
        }
    }
    return sum;
}
