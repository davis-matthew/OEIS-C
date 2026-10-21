/*
 * A294005  Number of multisets of exactly three nonempty words with a total
 *          of n letters over n-ary alphabet such that within each prefix of a
 *          word every letter of the alphabet is at least as frequent as the
 *          subsequent alphabet letter.
 * https://oeis.org/A294005
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n y^3] Prod_{j>=1} 1/(1-y*x^j)^A000085(j)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 20, u64 n <= 33, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
/* seq_t and idx_t are defined by the surrounding build. */

static seq_t *I_arr      = NULL;   /* Involution numbers up to max_n. */
static idx_t  max_I = 0;

/* Ensure I_arr contains involution numbers up to n. */
static void ensure_involutions(idx_t n)
{
    if (n <= max_I)
        return;

    if (I_arr == NULL) {
        I_arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    } else {
        I_arr = (seq_t *)realloc(I_arr, (n + 1) * sizeof(seq_t));
    }

    I_arr[0] = (seq_t)1;
    if (n >= 1)
        I_arr[1] = (seq_t)1;

    for (idx_t j = 2; j <= n; ++j) {
        seq_t prev1 = I_arr[j - 1];
        seq_t prev2 = I_arr[j - 2];
        I_arr[j] = prev1 + ((seq_t)(j - 1)) * prev2;
    }
    max_I = n;
}

/* Compute binomial coefficient C(a,2) with a = I+1. */
static seq_t binom2_from_I(idx_t index)
{
    seq_t a = I_arr[index] + 1;
    return (a * (a - 1)) / 2;
}

/* Compute binomial coefficient C(a,3) with a = I+2. */
static seq_t binom3_from_I(idx_t index)
{
    seq_t a = I_arr[index] + 2;
    return (a * (a - 1) * (a - 2)) / 6;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    ensure_involutions(n);

    seq_t term1 = 0, term2 = 0, term3 = 0;

    /* Case 1: three words of the same length */
    if (n % 3 == 0) {
        idx_t j = n / 3;
        term1 = binom3_from_I(j);
    }

    /* Case 2: a double word of length j and a single word of length k */
    idx_t max_j_case2 = (n - 1) / 2;
    for (idx_t j = 1; j <= max_j_case2; ++j) {
        idx_t k = n - 2 * j;
        if (k < 1 || k == j)
            continue;
        seq_t comb = binom2_from_I(j);
        term2 += comb * I_arr[k];
    }

    /* Case 3: three distinct lengths i < j < k */
    for (idx_t i = 1; i <= n - 2; ++i) {
        idx_t max_j = (n - i - 1) / 2;
        for (idx_t j = i + 1; j <= max_j; ++j) {
            idx_t k = n - i - j;
            /* By construction k > j, so no extra check needed */
            term3 += I_arr[i] * I_arr[j] * I_arr[k];
        }
    }

    return term1 + term2 + term3;
}
