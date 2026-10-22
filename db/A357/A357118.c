/*
 * A357118  Numbers such that the first digit is the number of digits and the
 *          second digit is the number of distinct digits.
 * https://oeis.org/A357118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   a(n) = n-th smallest integer whose first digit equals the *
 *            number of digits and whose second digit equals the number of
 *            distinct * digits of the integer. * * The algorithm iterates
 *            over the possible lengths (3..9) and the * possible
 *            distinct‑digit counts k. For each (len,k) pair the number *
 *            of...
 * Range:     u8 none, u16 n <= 710, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Binomial coefficients C[n][k] for 0 ≤ n,k ≤ 10 */
static unsigned long long C[11][11];

/* Power: base^exp  (base ≤ 9, exp ≤ 7) */
static unsigned long long pow_uint(unsigned long long base, int exp)
{
    unsigned long long res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* Precompute C[n][k] once */
static void init_binomial(void)
{
    for (int n = 0; n <= 10; ++n) {
        C[n][0] = C[n][n] = 1;
        for (int k = 1; k < n; ++k)
            C[n][k] = C[n-1][k-1] + C[n-1][k];
    }
}

/* Count how many extensions exist for given state:
 * remaining  positions to fill,
 * mask          current digit mask,
 * distinct      digits already used,
 * target_k      required final distinct count.
 */
static unsigned long long count_completions(int remaining,
                                            unsigned mask,
                                            unsigned distinct,
                                            unsigned target_k)
{
    int need = (int)target_k - (int)distinct;
    if (need < 0 || need > remaining) return 0;
    int unused = 10 - __builtin_popcount(mask);
    if (need > unused) return 0;

    /* Inclusion–exclusion over the 'need' new digits that have to appear */
    long long sum = 0;
    for (int i = 0; i <= need; ++i) {
        unsigned long long term = C[need][i] *
                                  pow_uint((unsigned long long)(target_k - i), remaining);
        if (i & 1) sum -= (long long)term;
        else       sum += (long long)term;
    }
    unsigned long long new_choose = C[unused][need];
    return (unsigned long long)sum * new_choose;
}

/* Total number of valid numbers of given length d and distinct count k */
static unsigned long long count_group(int d, int k)
{
    unsigned mask = (1u << d) | (1u << k);
    unsigned distinct = __builtin_popcount(mask);
    return count_completions(d - 2, mask, distinct, k);
}

/* The public function required by the problem statement */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static const idx_t offset = 1; /* a(1) is first term */

    if (!initialized) {
        init_binomial();
        initialized = true;
    }

    /* locate the (d,k) block that contains the n-th term */
    idx_t r = (idx_t)(n - offset);   /* 0‑based rank */
    for (int d = 3; d <= 9; ++d) {
        int maxk = d <= 10 ? d : 10;
        for (int k = 2; k <= maxk; ++k) {   /* k=1 impossible */
            unsigned long long cnt = count_group(d, k);
            if (r < cnt) {
                /* build the r‑th number in this block */
                seq_t value = (seq_t)d * 10 + (seq_t)k;
                unsigned mask = (1u << d) | (1u << k);
                unsigned distinct = __builtin_popcount(mask);
                int pos = 3;    /* next position to fill */

                while (pos <= d) {
                    for (int dig = 0; dig <= 9; ++dig) {
                        unsigned nmask = mask | (1u << dig);
                        unsigned add = (mask & (1u << dig)) ? 0 : 1;
                        unsigned ndistinct = distinct + add;

                        if (ndistinct > k) continue;

                        int rem_pos = d - pos;   /* remaining after this digit */
                        unsigned long long pref = count_completions(rem_pos,
                                                                     nmask,
                                                                     ndistinct,
                                                                     k);
                        if (r >= pref) {
                            r -= pref;
                            continue;
                        }

                        /* choose this digit */
                        value = value * 10 + (seq_t)dig;
                        mask = nmask;
                        distinct = ndistinct;
                        break;   /* exit dig loop */
                    }
                    ++pos;
                }
                return value;
            } else {
                r -= cnt;
            }
        }
    }
    /* n beyond the finite sequence – should never happen */
    return (seq_t)0;
}
