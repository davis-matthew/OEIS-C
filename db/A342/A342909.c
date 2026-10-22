/*
 * A342909  a(1)=1. Thereafter, if a(n) is a novel term a(n+1) = number of
 *          prior terms > a(n); otherwise a(n+1) = number of prior terms <=
 *          a(n).
 * https://oeis.org/A342909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(1) = 1. For n > 1: if a(n-1) is novel (appeared only once so
 *            far), a(n) = number of prior terms > a(n-1); else a(n) = number
 *            of prior terms <= a(n-1). (Prior terms are those with indices <
 *            n-1.)
 * Range:     u8 n <= 302, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined; return 0 for safety */

    /* convert to native integer for loop bounds; values here are small */
    size_t N = (size_t)n;

    /* allocate arrays of size N+2 (indices 0..N+1 are safe) */
    seq_t *seq = (seq_t *)calloc(N + 2, sizeof(seq_t));
    size_t *freq = (size_t *)calloc(N + 2, sizeof(size_t));
    uint32_t *bit = (uint32_t *)calloc(N + 3, sizeof(uint32_t)); /* BIT uses 1‑based indices */

    /* base case a(1) = 1 */
    seq[1] = (seq_t)1;
    size_t v = (size_t)(idx_t)seq[1];
    freq[v] = 1;
    /* add value 1 into BIT */
    for (size_t i = v + 1; i < N + 3; i += i & -i) bit[i] += 1;

    /* generate terms up to n */
    for (size_t i = 2; i <= N; ++i) {
        seq_t prev = seq[i - 1];
        size_t pv = (size_t)(idx_t)prev;              /* value of a(i‑1) as an index */
        bool novel = (freq[pv] == 1);                 /* no earlier occurrence */

        /* sum of frequencies for values <= pv (inclusive) */
        size_t sum = 0;
        for (size_t j = pv + 1; j > 0; j -= j & -j) sum += bit[j];
        size_t leq_excl = sum - 1;                    /* exclude the current term itself */

        size_t prior_terms = i - 2;                    /* terms with indices < i‑1 */
        size_t next_val;
        if (novel)
            next_val = prior_terms - leq_excl;         /* count of terms > pv */
        else
            next_val = leq_excl;                       /* count of terms <= pv */

        seq[i] = (seq_t)next_val;
        freq[next_val] += 1;
        /* insert next_val into BIT */
        for (size_t j = next_val + 1; j < N + 3; j += j & -j) bit[j] += 1;
    }

    seq_t result = seq[N];
    free(seq);
    free(freq);
    free(bit);
    return result;
}
