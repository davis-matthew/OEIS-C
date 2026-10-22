/*
 * A342157  Base-10 numbers m such that k*m = d||d||...||d (here || appears
 *          k-1 times), where k is the length of m, d is any m's digit and ||
 *          represents concatenation.
 * https://oeis.org/A342157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A342157
    Numbers m such that for k = number of decimal digits of m,
    there exists a digit d appearing in m so that k·m = d||d||⋯||d (k times).
    Computation: iterate over all possible lengths k = 1…9 and digits d = 0…9,
    compute m = d·(10^k−1)/(9k) and keep those where the division is exact,
    m has exactly k decimal digits and contains the digit d.
    The qualifying m values are sorted ascending. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* comparison function for qsort */
static int seq_cmp(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* Helper: does m contain digit d? */
static bool contains_digit(uint64_t m, uint8_t d)
{
    do {
        if ((uint8_t)(m % 10) == d)
            return true;
        m /= 10;
    } while (m);
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static buffer to hold the finite sequence */
    static seq_t seq[100];
    static size_t seq_len;
    static bool initialized = false;

    if (!initialized) {
        /* precompute 10^k for k=0..9 */
        static const uint64_t pow10_u64[10] = {
            1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL,
            100000ULL, 1000000ULL, 10000000ULL,
            100000000ULL, 1000000000ULL
        };

        seq_len = 0; /* number of found elements */

        for (idx_t k = 1; k <= 9; ++k) {
            for (idx_t d = 0; d <= 9; ++d) {
                seq_t numerator   = (seq_t)d * ((seq_t)pow10_u64[k] - 1);
                seq_t denominator = 9 * k;
                if (numerator % denominator != 0)
                    continue;

                seq_t m = numerator / denominator;

                /* Must have exactly k decimal digits */
                seq_t lower = (k == 1) ? 0 : (seq_t)pow10_u64[k - 1];
                seq_t upper = (seq_t)pow10_u64[k] - 1;
                if (m < lower || m > upper)
                    continue;

                /* m must contain the digit d */
                if (!contains_digit((uint64_t)m, (uint8_t)d))
                    continue;

                seq[seq_len++] = m;
            }
        }

        /* sort the list */
        qsort(seq, seq_len, sizeof(seq_t), seq_cmp);

        initialized = true;
    }

    if (n == 0 || n > seq_len)
        return (seq_t)0;                 /* undefined values return 0 */

    return seq[n - 1];
}
