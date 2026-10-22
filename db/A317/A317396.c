/*
 * A317396  Positive integers that have exactly six representations of the
 *          form 1 + p1 * (1 + p2* ... * (1 + p_j)...), where [p1, ..., p_j]
 *          is a (possibly empty) list of distinct primes.
 * https://oeis.org/A317396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sequence A317396 – numbers with exactly six representations of 1
 *            + p1*(1 + p2*(1 + …)) where the p’s are distinct primes
 * Range:     u8 none, u16 n <= 5392, u32 n <= 9462, u64 n <= 9462,
 *            u128 n <= 9462, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* upper bound for values we will generate – chosen large enough to contain
   the first 10000 terms.  2 000 000 is safe for this sequence. */
#define MAX_VAL 2000000

/* helper: binary search – largest index with primes[idx] <= value */
static int upper_bound_idx(const uint64_t *primes, int prime_count, uint64_t value) {
    int lo = 0, hi = prime_count;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (primes[mid] <= value)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;        /* may be -1 if value < first prime */
}

/* recursive generation of all sequences of distinct primes */
static void dfs(uint64_t prod,
                uint64_t sum,
                const uint64_t *primes,
                int prime_count,
                bool *used,
                uint16_t *freq) {
    /* remaining sum we can still add */
    uint64_t limit_prime = (MAX_VAL - sum) / prod;
    int upper = upper_bound_idx(primes, prime_count, limit_prime);
    if (upper < 0) return;
    for (int j = 0; j <= upper; ++j) {
        if (used[j]) continue;
        uint64_t nprod = prod * primes[j];
        uint64_t nsum  = sum + nprod;
        if (nsum > MAX_VAL) continue;
        ++freq[nsum];
        used[j] = true;
        dfs(nprod, nsum, primes, prime_count, used, freq);
        used[j] = false;
    }
}

seq_t a(idx_t n) {
    /* static data */
    static bool initialized = false;
    static seq_t *terms = NULL;   /* list of terms with exactly 6 representations */
    static size_t terms_len = 0;   /* length of terms array */

    if (!initialized) {
        /* ---------- sieve primes up to MAX_VAL ---------------------- */
        bool *is_prime = (bool *)malloc((MAX_VAL + 1) * sizeof(bool));
        memset(is_prime, true, (MAX_VAL + 1) * sizeof(bool));
        is_prime[0] = is_prime[1] = false;
        for (uint64_t i = 2; i * i <= MAX_VAL; ++i)
            if (is_prime[i])
                for (uint64_t j = i * i; j <= MAX_VAL; j += i)
                    is_prime[j] = false;

        int prime_count = 0;
        for (uint64_t i = 2; i <= MAX_VAL; ++i)
            if (is_prime[i]) ++prime_count;
        uint64_t *primes = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
        int idx = 0;
        for (uint64_t i = 2; i <= MAX_VAL; ++i)
            if (is_prime[i]) primes[idx++] = i;

        /* ---------- frequency array for representations (0 … MAX_VAL)  */
        uint16_t *freq = (uint16_t *)calloc((MAX_VAL + 1), sizeof(uint16_t));

        /* ---------- arrays for used primes during DFS  */
        bool *used = (bool *)calloc(prime_count, sizeof(bool));

        /* ---------- generate all sequences --------------------------- */
        for (int i = 0; i < prime_count; ++i) {
            uint64_t p     = primes[i];
            uint64_t s     = 1 + p;            /* sequence of length 1 */
            if (s > MAX_VAL) continue;
            ++freq[s];                         /* count length‑1 representation */

            used[i] = true;
            dfs(p, s, primes, prime_count, used, freq);
            used[i] = false;
        }

        /* ---------- build list of numbers with exactly 6 reps -------- */
        /* allocate rough capacity; will grow if needed */
        size_t cap = 16384;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        terms_len = 0;
        for (uint64_t v = 2; v <= MAX_VAL; ++v) {
            if (freq[v] == 6) {
                if (terms_len == cap) {
                    cap *= 2;
                    terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                }
                terms[terms_len++] = (seq_t)v;
            }
        }

        /* clean up */
        free(is_prime);
        free(primes);
        free(freq);
        free(used);

        initialized = true;
    }

    /* ---------- answer query -------------------------------------- */
    if (n == 0) return 0;          /* safety */
    if (n - 1 < terms_len)
        return terms[n - 1];
    else
        return 0; /* not found – should not happen for tested n */
}
