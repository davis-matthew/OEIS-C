/*
 * A294283  Sum of the larger parts of the partitions of n into two distinct
 *          parts with smaller part squarefree.
 * https://oeis.org/A294283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{i=1..⌊(n-1)/2⌋} (n-i) * μ(i)^2 μ(i)^2 is 1 iff i is
 *            square‑free
 * Range:     u8 n <= 31, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) using prefix sums of square‑free indicators.  */
#include <stdlib.h>

/* Integer square‑root (floor) for idx_t. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t i = 0;
    while (i + 1 <= n / (i + 1))
        ++i;
    return i;
}

seq_t a(idx_t n)
{
    if (n < 3)          /* a(1)=a(2)=0 */
        return 0;

    idx_t m = (n - 1) / 2;   /* upper limit of the sum */

    /* Static cache of prefix sums up to maxM. */
    static idx_t maxM = 0;
    static seq_t *prefCount = NULL;   /* Σ_{i≤j} is_squarefree(i) */
    static seq_t *prefWeight = NULL;  /* Σ_{i≤j} i·is_squarefree(i) */
    static unsigned char *sqf = NULL; /* 0/1 indicator */

    if (m > maxM) {
        idx_t newM = m;

        /* Resize arrays. */
        prefCount = (seq_t *)realloc(prefCount, (newM + 1) * sizeof(seq_t));
        prefWeight = (seq_t *)realloc(prefWeight, (newM + 1) * sizeof(seq_t));
        sqf = (unsigned char *)realloc(sqf, (newM + 1) * sizeof(unsigned char));

        if (!prefCount || !prefWeight || !sqf)
            exit(1);               /* no memory */

        /* Initialize all numbers as square‑free (1). */
        sqf[0] = 0;                /* unused */
        for (idx_t i = 1; i <= newM; ++i)
            sqf[i] = 1;

        /* Mark multiples of squares. */
        idx_t limit = floor_sqrt_u64(newM);
        for (idx_t p = 2; p <= limit; ++p) {
            idx_t sq = p * p;
            for (idx_t j = sq; j <= newM; j += sq)
                sqf[j] = 0;
        }

        /* Build prefix sums. */
        prefCount[0] = 0;
        prefWeight[0] = 0;
        for (idx_t i = 1; i <= newM; ++i) {
            prefCount[i] = prefCount[i - 1] + sqf[i];
            prefWeight[i] = prefWeight[i - 1] + ((seq_t)i * sqf[i]);
        }

        maxM = newM;
    }

    /* a(n) = n * count_squarefree(m) - sum_i_squarefree(i) */
    return ((seq_t)n * prefCount[m]) - prefWeight[m];
}
