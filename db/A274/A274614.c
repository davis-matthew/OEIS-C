/*
 * A274614  Third column of array in A269526.
 * https://oeis.org/A274614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A274615(n-1) + 1, where A274615(0) = 1 and, for m >= 1
 *            with m = 16t + i (0 <= i <= 15), A274615(m) = 16t + c[i], c =
 *            {-1,4,5,2,0,8,3,6,7,12,13,10,11,9,17,14}
 * Range:     u8 n <= 254, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 83 terms through n = 980 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A274615_a(idx_t n)
{
    static const int c[16] = {
        -1, 4, 5, 2, 0, 8, 3, 6,
         7,12,13,10,11, 9,17,14
    };
    if (n == 0) return (seq_t)1;
    idx_t i = n % 16;
    seq_t base = (seq_t)(n - i);
    if (i == 0)
        return base - (seq_t)1;
    return base + (seq_t)c[i];
}

seq_t a(idx_t n)
{
    return A274615_a(n - 1) + (seq_t)1;
}
