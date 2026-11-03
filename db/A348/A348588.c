/*
 * A348588  Number of partitions of n into 3 parts with at least 1 prime part.
 * https://oeis.org/A348588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 66 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A348588: Number of partitions of n into 3 parts with at least 1 prime part
 *
 * Algorithm: 
 *   Total number of unordered partitions of n into 3 positive parts is 
 *   floor((n^2 + 6)/12).  We subtract the number of those partitions that
 *   have no prime parts (i.e., all parts are composite or 1).  The latter
 *   is counted by enumerating all non‑decreasing triples (a,b,c) with a,b,c
 *   composite (or 1), summing only those with a+b+c == n.
 *
 *   A simple sieve of Eratosthenes is used to test primality up to the
 *   maximum n received by the function; the sieve is recomputed only
 *   when a larger n is requested.  The enumeration loops run in
 *   O(n^2) time, which is well below 2 s for n ≤ 10000.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Helper to update the prime sieve up to at least limit */
static void update_sieve(idx_t limit, bool **prime, idx_t *sieve_size)
{
    if (*sieve_size >= limit) return;
    /* Reallocate */
    bool *new_flag = (bool *)realloc(*prime, (limit + 1) * sizeof(bool));
    if (!new_flag) return; /* allocation failure – unlikely for small limits */
    *prime = new_flag;
    /* Initialize: assume all >=2 are prime */
    for (idx_t i = 2; i <= limit; ++i) (*prime)[i] = true;
    /* Sieve */
    for (idx_t p = 2; p * p <= limit; ++p)
        if ((*prime)[p])
            for (idx_t j = p * p; j <= limit; j += p)
                (*prime)[j] = false;
    *sieve_size = limit;
}

seq_t a(idx_t n)
{
    /* Handle trivial cases quickly */
    if (n == 0) return 0;

    /* Calculate total number of partitions of n into 3 parts */
    seq_t n_seq = (seq_t)n;
    seq_t total = (n_seq * n_seq + 6) / 12;

    /* Prime sieve for primality tests */
    static bool *is_prime = NULL;
    static idx_t sieve_max = 0;
    update_sieve(n, &is_prime, &sieve_max);

    /* Count partitions with no prime parts */
    seq_t bad = 0;
    for (idx_t a_val = 1; a_val <= n / 3; ++a_val) {
        if (a_val >= 2 && is_prime[a_val]) continue;  /* skip primes */
        for (idx_t b_val = a_val; b_val <= (n - a_val) / 2; ++b_val) {
            if (b_val >= 2 && is_prime[b_val]) continue;
            idx_t c_val = n - a_val - b_val;
            if (c_val < b_val) continue;          /* maintain non‑decreasing order */
            if (c_val >= 2 && is_prime[c_val]) continue;
            bad += 1;
        }
    }

    return total - bad;
}
