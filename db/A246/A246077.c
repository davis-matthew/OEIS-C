/*
 * A246077  Paradigm shift sequence for (-1,-3) production scheme with
 *          replacement.
 * https://oeis.org/A246077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-5) for n > 15, with initial values a(1) = 1, a(2) =
 *            2, a(3) = 3, a(4) = 4, a(5) = 5, a(6) = 6, a(7) = 7, a(8) = 8,
 *            a(9) = 9, a(10) = 10, a(11) = 12, a(12) = 14, a(13) = 16, a(14)
 *            = 18, a(15) = 21
 * Range:     u8 n <= 32, u16 n <= 72, u32 n <= 152, u64 n <= 312,
 *            u128 n <= 632, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t base_values[16] = {
    0ULL,          // unused index 0
    1ULL, 2ULL, 3ULL, 4ULL, 5ULL,          // 1–5
    6ULL, 7ULL, 8ULL, 9ULL, 10ULL,        // 6–10
    12ULL, 14ULL, 16ULL, 18ULL, 21ULL     // 11–15
};

seq_t a(idx_t n)
{
    if (n <= 15) {
        return base_values[n];
    }

    /* Number of times the recurrence must be applied to bring
       the index down to ≤ 15.  This is ceil((n-15)/5). */
    idx_t k = (n - 15 + 4) / 5;
    idx_t L = n - 5 * k;       /* 11 ≤ L ≤ 15 */

    seq_t res = base_values[L];
    for (idx_t i = 0; i < k; ++i) {
        res *= 2;
    }
    return res;
}
