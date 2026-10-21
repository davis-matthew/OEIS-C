/*
 * A291252  p-INVERT of (0,1,0,1,0,1,...), where p(S) = (1 - S^3)^3.
 * https://oeis.org/A291252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the p‑INVERT of (0,1,0,1,0,1,…), p(S) = (1 – S³)³.
 *            Recurrence for n ≥ 19: a(n) = 9a(n‑2) + 3a(n‑3) – 36a(n‑4) –
 *            18a(n‑5) + 81a(n‑6) + 45a(n‑7) – 117a(n‑8) – 59a(n‑9) +
 *            117a(n‑10) + 45a(n‑11) – 81a(n‑12) – 18a(n‑13) + 36a(n‑14) +
 *            3a(n‑15) – 9a(n‑16) + a(n‑18) Initial values: a(0)=0, a(1)=0...
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 41, u64 n <= 85, u128 n <= 174,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[19] = {
    0ULL, 0ULL, 3ULL, 0ULL, 9ULL, 6ULL, 18ULL, 36ULL, 40ULL, 126ULL,
    135ULL, 351ULL, 513ULL, 936ULL, 1755ULL, 2682ULL, 5373ULL, 8260ULL,
    15525ULL
};

seq_t a(idx_t n)
{
    /* Quick return for the first 19 terms */
    if (n < 19ULL)
        return init[(size_t)n];

    /* Allocate space for the sequence up to n */
    size_t N = (size_t)n;
    seq_t *seq = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!seq)
        exit(1);  /* out of memory */

    /* Copy initial values */
    for (size_t i = 0; i < 19; ++i)
        seq[i] = init[i];

    /* Compute subsequent terms using the linear recurrence */
    for (size_t i = 19; i <= N; ++i) {
        seq[i] =  9 * seq[i - 2]
                + 3 * seq[i - 3]
                - 36 * seq[i - 4]
                - 18 * seq[i - 5]
                + 81 * seq[i - 6]
                + 45 * seq[i - 7]
                - 117 * seq[i - 8]
                - 59  * seq[i - 9]
                + 117 * seq[i - 10]
                + 45  * seq[i - 11]
                - 81  * seq[i - 12]
                - 18  * seq[i - 13]
                + 36  * seq[i - 14]
                + 3   * seq[i - 15]
                - 9   * seq[i - 16]
                + seq[i - 18];
    }

    seq_t result = seq[N];
    free(seq);
    return result;
}
