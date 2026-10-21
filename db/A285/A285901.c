/*
 * A285901  Total number of partitions of all positive integers <= n into an
 *          odd number of consecutive parts.
 * https://oeis.org/A285901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{odd d≤n} max(0, ⌊n/d⌋ – ⌊d/2⌋)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) counts for each integer k≤n the odd divisors d of k
    that satisfy d < √(2k).  For a fixed odd divisor d the
    admissible multiples k are k = d·m with m > d/2,
    hence the number of such k up to n is
         max(0, ⌊n/d⌋ – ⌊d/2⌋).  Summing over all odd d
    yields the desired sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for offset 1 */
    seq_t total = 0;
    for (idx_t d = 1; d <= n; d += 2) {
        idx_t cnt  = n / d;        /* ⌊n/d⌋  */
        idx_t half = d / 2;        /* ⌊d/2⌋  */
        if (cnt > half) {
            total += (seq_t)(cnt - half);
        }
    }
    return total;
}
