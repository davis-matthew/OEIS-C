/*
 * A346299  Positions of words in A076478 in which #0's < #1's.
 * https://oeis.org/A346299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   positions of binary words (enumerated by increasing length,
 *            lexicographically) with fewer 0's than 1's. The sequence is
 *            defined as the indices of such words in A076478. The
 *            implementation finds, for a given rank n, the corresponding word
 *            by combinatorial counting and returns its overall position
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t binom_seq(int n, int k)
{
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (int i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

extern seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* out of range, may return 0 */

    seq_t cumGood = 0;               /* cumulative number of good words so far */
    int L = 0;                       /* current length being examined */

    /* Find the length L of the word that contains the nth good word */
    while (1) {
        ++L;
        seq_t g;
        if (L & 1) {
            g = ((seq_t)1 << (L - 1));                     /* odd length */
        } else {
            seq_t mid = binom_seq(L, L / 2);               /* even length */
            g = ((seq_t)1 << (L - 1)) - (mid >> 1);
        }
        if (cumGood + g >= n) break;                      /* found the length */
        cumGood += g;
    }

    seq_t offset = n - cumGood;                           /* rank within length L */
    int thresholdZeros = (L - 1) / 2;                      /* floor((L-1)/2) */

    seq_t rank = 0;                                       /* lexicographic rank within length L */
    int zeros = 0;                                         /* zeros used so far */

    for (int pos = 0; pos < L; ++pos) {
        int rem = L - pos - 1;                              /* bits still to decide */

        /* Count good completions if we put a '0' at this position */
        int zeros_new0 = zeros + 1;
        seq_t cnt0 = 0;
        if (zeros_new0 <= thresholdZeros) {
            int allowed = thresholdZeros - zeros_new0;
            seq_t comb = 1;                                 /* C(rem,0) */
            cnt0 += comb;
            for (int k = 1; k <= allowed; ++k) {
                comb = comb * (rem - k + 1) / k;            /* C(rem,k) from previous */
                cnt0 += comb;
            }
        } else {
            cnt0 = 0;
        }

        if (offset <= cnt0) {
            /* Choose bit 0 */
            zeros = zeros_new0;
            /* rank unchanged */
        } else {
            /* Choose bit 1 */
            offset -= cnt0;
            /* Skip all strings that would have 0 at this position */
            rank += ((seq_t)1 << rem);
            /* zeros unchanged (bit 1 adds no zero) */
        }
    }

    seq_t lex_rank = rank + 1;                               /* 1‑based lexicographic rank */

    /* Compute the number of strings of all shorter lengths (the base position) */
    seq_t pow2_L_minus1 = ((seq_t)1 << (L - 1));
    seq_t base_pos = pow2_L_minus1 * 2 - 2;                  /* 2^L - 2 */

    return base_pos + lex_rank;
}
