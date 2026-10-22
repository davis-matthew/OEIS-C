/*
 * A341795  Number of ways to write n as an ordered sum of 4 nonzero
 *          tetrahedral numbers.
 * https://oeis.org/A341795
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered 4‑tuples of positive integers (i,j,k,l)
 *            with binomial(i+2,3)+binomial(j+2,3)+binomial(k+2,3)+
 *            binomial(l+2,3)=n
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation uses two‑sum convolution:
    first compute all ordered pairs of tetrahedral numbers whose sum
    does not exceed n, count the occurrences in an array freq.
    Then a(n) is the sum over all s of freq[s] * freq[n‑s].   */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 4) {
        return 0;
    }

    /* find the largest k with binomial(k+2,3) <= n */
    idx_t k = 1;
    for (;;) {
        seq_t tk = (seq_t)k * ((seq_t)k + 1) * ((seq_t)k + 2) / 6;
        if (tk > n) {
            break;
        }
        ++k;
    }
    idx_t m = k - 1;            /* number of tetrahedral numbers to use */

    /* allocate array of tetrahedral numbers */
    seq_t *T = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    if (!T) {
        return 0;
    }
    for (idx_t i = 1; i <= m; ++i) {
        T[i] = (seq_t)i * ((seq_t)i + 1) * ((seq_t)i + 2) / 6;
    }

    /* allocate frequency array for pair sums up to n */
    seq_t *freq = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!freq) {
        free(T);
        return 0;
    }

    /* count ordered pairs (i,j) with T[i]+T[j] <= n */
    for (idx_t i = 1; i <= m; ++i) {
        seq_t Ti = T[i];
        for (idx_t j = 1; j <= m; ++j) {
            seq_t sum = Ti + T[j];
            if (sum <= n) {
                ++freq[(size_t)sum];
            }
        }
    }

    /* convolution: sum over s of freq[s] * freq[n-s] */
    seq_t result = 0;
    for (idx_t s = 0; s <= n; ++s) {
        seq_t f1 = freq[(size_t)s];
        if (!f1) {
            continue;
        }
        seq_t f2 = freq[(size_t)(n - s)];
        if (!f2) {
            continue;
        }
        result += f1 * f2;
    }

    free(T);
    free(freq);
    return result;
}
