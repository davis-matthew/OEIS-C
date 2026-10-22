/*
 * A302872  Number of nX3 0..1 arrays with every element equal to 0, 1, 3, 4
 *          or 5 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 9*a(n-3) – 15*a(n-4) – 5*a(n-5) – 22*a(n-6) –
 *            7*a(n-7) + 9*a(n-8) – 32*a(n-9) – 10*a(n-10) – 16*a(n-11) –
 *            40*a(n-12) – 16*a(n-13), for n > 16 Initial values: a(1) = 3,
 *            a(2) = 11, a(3) = 17, a(4) = 37, a(5) = 82, a(6) = 209, a(7) =
 *            536, a(8) = 1549, a(9) = 4513, a(10) = 13980, a(...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* pre‑computed values for 1 ≤ n ≤ 16 */
    static const seq_t base[17] = {
        0ULL,          /* base[0] unused */
        3ULL, 11ULL, 17ULL, 37ULL, 82ULL, 209ULL, 536ULL,
        1549ULL, 4513ULL, 13980ULL, 44424ULL, 142457ULL,
        463592ULL, 1518144ULL, 4985864ULL, 16423493ULL
    };

    if (n <= 16) return base[n];

    /* array to hold sequence values up to n */
#ifdef __cplusplus
    std::vector<seq_t> seq(n + 1);
#else
    seq_t *seq = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!seq) return 0;   /* allocation failure – unlikely in tests */
#endif

    /* initialise known values */
    for (idx_t i = 1; i <= 16; ++i) seq[i] = base[i];

    /* compute a(n) for n > 16 using the recurrence */
    for (idx_t k = 17; k <= n; ++k) {
        seq_t pos = 3 * seq[k - 1] + 9 * seq[k - 3] + 9 * seq[k - 8];
        seq_t neg = 15 * seq[k - 4] + 5 * seq[k - 5] + 22 * seq[k - 6] +
                    7 * seq[k - 7] + 32 * seq[k - 9] + 10 * seq[k - 10] +
                    16 * seq[k - 11] + 40 * seq[k - 12] + 16 * seq[k - 13];
        seq[k] = pos - neg;
    }

    seq_t result = seq[n];
#ifdef __cplusplus
    /* std::vector will free its storage automatically */
#else
    free(seq);
#endif
    return result;
}
