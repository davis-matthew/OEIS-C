/*
 * A382771  Number of ways to permute the prime indices of n so that the
 *          run-lengths are all different.
 * https://oeis.org/A382771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations of the multiset of * prime indices
 *            of n such that all run‑lengths are distinct
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Forward declaration of recursive search */
static void dfs_rec(const int *counts, int k, int last,
                    uint16_t used_mask, int remaining, __attribute__((unused)) seq_t *total);

/* Sequence function */
seq_t a(idx_t n)
{
    /* n = 1 has no prime factors → one trivial permutation */
    if (n <= 1)
        return 1;

    /* Factorization: n <= 10 000, so simple trial division is fine. */
    uint64_t m = n;
    int exp[20];   /* maximum number of distinct primes for n <= 10 000 is < 10 */
    int k = 0;     /* number of distinct primes found */

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int cnt = 0;
            while (m % p == 0) {
                m /= p;
                ++cnt;
            }
            exp[k++] = cnt;
        }
    }
    if (m > 1) /* remaining prime factor */
        exp[k++] = 1;

    /* Sum of all exponents gives the total number of indices, i.e. the
     * number of items in the multiset.  It also bounds the maximum
     * run‑length. */
    int total = 0;
    for (int i = 0; i < k; ++i)
        total += exp[i];

    /* Count the number of valid permutations. */
    seq_t result = 0;
    dfs_rec(exp, k, k, 0U, total, &result);  /* last = k  → “no last prime” */
    return result;
}

/* ---------- Recursive helper ---------------------------------- */
static void dfs_rec(const int *counts, int k, int last,
                    uint16_t used_mask, int remaining, seq_t *total)
{
    if (remaining == 0) {
        ++(*total);
        return;
    }

    for (int i = 0; i < k; ++i) {
        if (counts[i] == 0 || i == last)
            continue;

        /* Try all possible run lengths for this prime that have not
         * been used yet.  Because run lengths go up to “remaining”
         * but are bounded by the exponent of this prime. */
        for (int len = 1; len <= counts[i]; ++len) {
            uint16_t bit = 1U << (len - 1);
            if (used_mask & bit)
                continue;

            /* Place a run of length “len” for prime i. */
            int new_counts[20];
            for (int j = 0; j < k; ++j)
                new_counts[j] = counts[j];
            new_counts[i] -= len;

            dfs_rec(new_counts, k, i,
                    used_mask | bit,
                    remaining - len,
                    total);
        }
    }
}
