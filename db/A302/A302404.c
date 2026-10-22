/*
 * A302404  Total domination number of the n-Moebius ladder.
 * https://oeis.org/A302404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 382, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-6) + 4
   Initial values: a(0)=0, a(1)=2, a(2)=2, a(3)=2, a(4)=3, a(5)=4
   The formula a(n) = (n/6)*4 + a(n mod 6) follows from this recurrence. */

seq_t a(idx_t n)
{
    static const seq_t base[6] = {0, 2, 2, 2, 3, 4};
    if (n < 6)
        return base[n];
    idx_t k = n / 6;
    idx_t r = n % 6;
    return base[r] + ((seq_t)k * (seq_t)4);
}
