/*
 * A374056  a(n) = max_{i=0..n} S_4(i) + S_4(n-i) where S_4(x) = A053737(x) is
 *          the base-4 digit sum of x.
 * https://oeis.org/A374056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 3*⌊log_4(n+1)⌋ + A053737(n+1) - 1
 * Range:     u8 n <= 385, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = max_{i=0..n} S_4(i) + S_4(n-i),  where S_4 is the base‑4 digit sum. */

seq_t a(idx_t n)
{
    /* n+1 in seq_t format */
    seq_t x = (seq_t)n + 1;

    /* sum of base‑4 digits and digit count */
    seq_t sum = 0;
    unsigned d = 0;
    seq_t r = x;
    while (r > 0) {
        sum += r & 3;   /* low two bits = r % 4 */
        r >>= 2;        /* integer division by 4 */
        d++;
    }

    /* floor(log_4(n+1)) = d - 1 because d is the number of base‑4 digits */
    seq_t res = 3 * (seq_t)(d - 1) + sum - 1;
    return res;
}
