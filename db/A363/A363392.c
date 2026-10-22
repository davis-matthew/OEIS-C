/*
 * A363392  Sum of divisors of 4*n-2 of form 4*k+3.
 * https://oeis.org/A363392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum of all divisors d of (4*n-2) with d ≡ 3 (mod 4)
 * Range:     u8 n <= 64, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the element a(n) of OEIS sequence A363392 for the supplied index n. */
seq_t a(idx_t n)
{
    seq_t sum = 0;                     /* running total of eligible divisors */
    seq_t m   = (seq_t)4 * n - 2;       /* the integer whose divisors we examine */

    /* For n = 1, m = 2 which has no divisor ≡ 3 (mod 4) */
    if (m < 3) return 0;

    /* Use 64‑bit loop counters since m is small for the intended range of n. */
    idx_t m64 = (idx_t)m;                /* m fits safely in an unsigned 64‑bit type */

    /* Enumerate divisors by checking 1 .. sqrt(m) and their complementary factors. */
    for (idx_t i = 1; i <= m64 / i; ++i) {
        if (m64 % i != 0) continue;       /* i is not a divisor */

        idx_t j = m64 / i;                /* complementary divisor */

        /* Check i (the smaller divisor). */
        if ((i & 3ULL) == 3ULL) {
            sum += (seq_t)i;
        }

        /* Check j unless it is equal to i (the perfect square case). */
        if (j != i && (j & 3ULL) == 3ULL) {
            sum += (seq_t)j;
        }
    }
    return sum;
}
