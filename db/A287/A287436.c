/*
 * A287436  Positions of 1 in A053838.
 * https://oeis.org/A287436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n-1) + j + 1, where j = (1 - A053838(n-1)) mod 3. Each
 *            block 3k..3k+2 holds exactly one number with A053838 = 1, and
 *            a(n) is that number plus 1 (the offset shift)
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9937 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A287436_A053838_a(idx_t n)
{
    seq_t sum = 0;
    while (n > 0) {
        sum += (seq_t)(n % 3);
        n /= 3;
    }
    return sum % 3;
}

seq_t a(idx_t n)
{
    idx_t k = n - 1;
    seq_t s = A287436_A053838_a(k);
    seq_t j = (seq_t)((seq_t)1 + (seq_t)3 - s) % 3;
    return (seq_t)3 * (seq_t)k + j + 1;
}
