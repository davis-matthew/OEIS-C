/*
 * A382654  a(1) = 1, for n > 1, a(n) = a(n - 1) / 2 if a(n - 1) is divisible
 *          by 2, otherwise a(n) = a(n - 1) + (first digit of (a(n - 1) + (n -
 *          1))).
 * https://oeis.org/A382654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1;
   for n > 1:
     if a(n‑1) is even, a(n) = a(n‑1) / 2;
     else a(n) = a(n‑1) + firstdigit(a(n‑1) + (n‑1)) */

static inline seq_t first_digit(seq_t v)
{
    seq_t ten = 10;
    while (v >= ten)
        v /= ten;
    return v;
}

seq_t a(idx_t n)
{
    seq_t cur = 1;
    if (n == 1)
        return cur;

    for (idx_t i = 2; i <= n; ++i) {
        if ((cur % 2) == 0) {
            cur /= 2;
        } else {
            seq_t temp = cur + (seq_t)(i - 1);
            cur += first_digit(temp);
        }
    }
    return cur;
}
