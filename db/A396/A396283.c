/*
 * A396283  a(n) = Sum_{k=1..n} (n mod k)^2.
 * https://oeis.org/A396283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (n mod k)^2
 * Range:     u8 n <= 18, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;
    idx_t k = 1;
    while (1) {                  /* use a loop that guards against uint64_t overflow */
        idx_t r = n % k;
        sum += (seq_t)r * (seq_t)r;
        if (k == n)              /* stop after the last term */
            break;
        ++k;
    }
    return sum;
}
