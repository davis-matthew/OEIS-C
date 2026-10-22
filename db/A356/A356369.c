/*
 * A356369  Numbers such that each digit "d" occurs d times, for every digit
 *          from 1 to the largest digit.
 * https://oeis.org/A356369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   a(n) is the n‑th term of the sequence of numbers whose digit d
 *            appears exactly d times for each d=1..m. The terms are sorted
 *            first by m (increasing) and within each m by lexicographic
 *            (hence numeric) order of the permutations of the multiset
 *            {1^1,2^2,…,m^m}
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 9304, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_from_str.h"

/* --------------------------------------------------------------------- */
/* binomial coefficient (small parameters, fits in 64 bits)              */
static idx_t binomial_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t r = 1;
    for (idx_t i = 1; i <= k; ++i)
        r = r * (n - k + i) / i;
    return r;
}

/* --------------------------------------------------------------------- */
/* multinomial coefficient for the multiset counts[1..m]                 */
static seq_t multinomial_count(const uint8_t cnt[10], uint8_t m)
{
    seq_t prod = 1;
    idx_t sum  = 0;
    for (uint8_t k = 1; k <= m; ++k) {
        sum += cnt[k];
        prod *= (seq_t)binomial_idx(sum, cnt[k]);
    }
    return prod;
}

/* --------------------------------------------------------------------- */
/* pre‑compute permutation counts for m = 1..9                           */
static seq_t perm_counts[10];   /* not strictly needed, kept for clarity */
static seq_t cum_counts[10];    /* cumulative counts up to m */
static bool initialized = false;

static void init_counts(void)
{
    if (initialized) return;
    cum_counts[0] = 0;
    for (uint8_t m = 1; m <= 9; ++m) {
        /* multiply ∏_{k=1..m} binom(S_k, k)   where S_k = k+...+m */
        seq_t prod = 1;
        idx_t sum  = 0;
        for (uint8_t k = 1; k <= m; ++k) {
            sum += k;                    /* S_k = k + (k+1) + … + m */
            prod *= (seq_t)binomial_idx(sum, k);
        }
        perm_counts[m] = prod;
        cum_counts[m] = cum_counts[m-1] + prod;
    }
    initialized = true;
}

/* --------------------------------------------------------------------- */
/* main function required by the task                                     */
seq_t a(idx_t n)
{
    if (n == 0) return seq_from_str("0");   /* undefined offset, arbitrary */

    init_counts();

    /* locate the block m such that cum_counts[m-1] < n ≤ cum_counts[m] */
    uint8_t m = 1;
    while (m <= 9 && (seq_t)n > cum_counts[m]) ++m;
    if (m > 9) m = 9;                       /* safety for out‑of‑range n */

    seq_t prev_cum   = cum_counts[m-1];
    idx_t idx_in_block = n - (idx_t)prev_cum - 1;   /* 0‑based index inside block */

    /* prepare digit counts: cnt[d] = d for d = 1..m */
    uint8_t cnt[10] = {0};
    for (uint8_t d = 1; d <= m; ++d) cnt[d] = d;

    /* total number of digits in this block */
    idx_t total_len = (idx_t)m * (m + 1) / 2;
    char digits[46];  /* maximum length 45 + 1 for null terminator */
    idx_t pos = 0;

    /* construct the idx_in_block‑th lexicographic permutation */
    while (pos < total_len) {
        for (uint8_t d = 1; d <= m; ++d) {
            if (cnt[d] == 0) continue;

            /* try to place digit d at position pos */
            cnt[d]--;

            seq_t rem_perms = multinomial_count(cnt, m);

            if ((seq_t)idx_in_block < rem_perms) {
                digits[pos++] = (char)('0' + d);
                break;                  /* digit chosen, move to next position */
            } else {
                /* skip all permutations starting with this digit */
                idx_in_block -= (idx_t)rem_perms;
                cnt[d]++;               /* put the digit back */
            }
        }
    }

    digits[pos] = '\0';
    return seq_from_str(digits);
}
