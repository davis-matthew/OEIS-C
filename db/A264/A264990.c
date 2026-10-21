/*
 * A264990  a(n) = number of occurrences of a most frequent nonzero digit in
 *          factorial base representation (A007623) of n.
 * https://oeis.org/A264990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximum frequency of any non‑zero digit in the factorial
 *            base representation of n (A007623)
 * Range:     u8 n <= 9710, u16 n <= 9710, u32 n <= 9710, u64 n <= 9710,
 *            u128 n <= 9710, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Count how many times each possible non‑zero digit occurs.
       The largest possible digit for a 64‑bit n is 21
       (for n < 2^64, pos ≤ 20 → digit < pos+1 ≤ 21). */
    unsigned short counts[22] = {0};

    idx_t temp = n;
    idx_t pos  = 1;          /* position i corresponds to divisor i+1 */

    while (temp != 0) {
        idx_t digit = temp % (pos + 1);
        temp /= (pos + 1);

        if (digit != 0) {
            /* digit fits into the array index (< 22) */
            counts[(unsigned)digit] += 1;
        }
        ++pos;
    }

    unsigned short best = 0;
    for (unsigned i = 1; i < 22; ++i) {
        if (counts[i] > best) best = counts[i];
    }
    return (seq_t)best;
}
