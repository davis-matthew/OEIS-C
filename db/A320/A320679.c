/*
 * A320679  Number of 4-element subsets of [n] having a prime element sum.
 * https://oeis.org/A320679
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4‑element subsets of [1..n] whose element sum
 *            is prime. The algorithm computes all unordered pairs of pairs
 *            with prime total sum and divides the total by 3, because each
 *            4‑set is counted exactly 3 times (it can be partitioned into two
 *            pairs in 3 different ways)
 * Range:     u8 n <= 11, u16 n <= 41, u32 n <= 941, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   overlap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----------   helpers   ---------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/overlap.h"

/* count of unordered pairs {i,j}, 1 ≤ i < j ≤ n, i + j = s */
static idx_t pair_count(idx_t s, idx_t n) {
    idx_t low = 1;
    if (s > n) low = s - n;                /*  j = s - i <= n  ->  i >= s-n */
    idx_t high = (s - 1) / 2;               /*  i < j  ->  i <= (s-1)/2  */
    if (high < low) return 0;
    return high - low + 1;
}

/* ----------   main function   ---------- */
seq_t a(idx_t n) {
    if (n < 4) return 0;          /* offset = 4 */

    /* pre‑compute pair numbers m_s for 2 ≤ s ≤ 2n */
    size_t m_size = (size_t)(2 * n + 1);
    idx_t *m = (idx_t *)calloc(m_size, sizeof(idx_t));
    if (!m) return 0;             /* allocation failure, unlikely */

    for (idx_t s = 2; s <= 2 * n; ++s)
        m[s] = pair_count(s, n);

    /* sieve primes up to 4n */
    idx_t max_sum = 4 * n;
    size_t sieve_size = (size_t)(max_sum + 1);
    bool *is_prime = (bool *)calloc(sieve_size, sizeof(bool));
    if (!is_prime) { free(m); return 0; }

    for (size_t i = 0; i <= max_sum; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t p = 2; p * p <= max_sum; ++p)
        if (is_prime[p])
            for (size_t j = p * p; j <= max_sum; j += p)
                is_prime[j] = false;

    seq_t total = 0;           /* raw count of unordered pair‑of‑pairs */

    /* iterate over primes p ≥ 10 (smallest possible 4‑sum) */
    for (size_t p = 10; p <= max_sum; ++p) {
        if (!is_prime[p]) continue;

        idx_t s_start = (p > 2 * n) ? (idx_t)(p - 2 * n) : 2;
        idx_t s_end   = (idx_t)(p / 2);
        if (s_end > 2 * n) s_end = 2 * n;

        if (s_start > s_end) continue;

        for (idx_t s = s_start; s <= s_end; ++s) {
            idx_t t = (idx_t)p - s;
            if (t < s) continue;                 /* safety */

            idx_t ms = m[s], mt = m[t];
            if (!ms || !mt) continue;            /* no pairs of that sum */

            seq_t val;
            if (s == t) {                        /* m_s choose 2 */
                val = ((seq_t)ms * (ms - 1)) / 2;
            } else {
                val = ((seq_t)ms * mt) - overlap(s, t, n);
            }
            total += val;
        }
    }

    free(m);
    free(is_prime);

    return total / 3;          /* each 4‑set counted exactly 3 times */
}
