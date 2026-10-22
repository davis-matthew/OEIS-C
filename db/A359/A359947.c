/*
 * A359947  a(n) = n XOR A359946(n) (where XOR denotes the bitwise XOR
 *          operator).
 * https://oeis.org/A359947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n XOR partner(n) * where partner(n) is the unique m such
 *            that n XOR m is prime * and the set of natural numbers is
 *            partitioned into unordered pairs * (n, partner(n)) by this rule,
 *            choosing for each smallest unpaired n * the smallest unpaired m
 *            > n with n XOR m prime
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Global state for the partner mapping */
static seq_t   *partner = NULL;   /* partner[n] = m in the pair {n,m} */
static size_t   cap = 0;          /* allocated size of partner array */
static idx_t    built_up_to = 0;  /* maximum n that has been paired */

/* Prime sieve state */
static bool    *prime_sieve = NULL;   /* prime_sieve[p] == true iff p is prime */
static idx_t    prime_limit = 0;      /* sieve covers 0..prime_limit */

/* Ensure partner array covers at least indices up to n_needed */
static void ensure_partner_cap(idx_t n_needed) {
    if (n_needed + 1 <= cap) return;
    size_t new_cap = cap ? cap * 2 : 32;
    while (new_cap <= (size_t)n_needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(partner, new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);          /* allocation failure */
    if (new_cap > cap)
        memset(new_arr + cap, 0,
               (new_cap - cap) * sizeof(seq_t));
    partner = new_arr;
    cap = new_cap;
}

/* Build prime sieve up to at least 'limit' */
static void ensure_prime_limit(idx_t limit) {
    if (limit <= prime_limit) return;
    bool *new_sieve = (bool *)malloc((size_t)(limit + 1));
    if (!new_sieve) exit(EXIT_FAILURE);
    memset(new_sieve, true, (size_t)(limit + 1));
    new_sieve[0] = new_sieve[1] = false;
    for (idx_t p = 2; p * p <= limit; ++p)
        if (new_sieve[p])
            for (idx_t mult = p * p; mult <= limit; mult += p)
                new_sieve[mult] = false;
    free(prime_sieve);
    prime_sieve = new_sieve;
    prime_limit = limit;
}

/* Fast prime test using the sieve or trial division */
static bool is_prime_fast(idx_t x) {
    if (x <= prime_limit) return prime_sieve[x];
    if (x < 2) return false;
    for (idx_t d = 2; d * d <= x; ++d)
        if (x % d == 0) return false;
    return true;
}

/* Pair unpaired numbers from built_up_to+1 up to up_to */
static void build_pairs_upto(idx_t up_to) {
    for (idx_t i = built_up_to + 1; i <= up_to; ++i) {
        if (partner[i] != 0) continue;  /* already paired */
        idx_t j = i + 1;
        for (;;) {
            ensure_partner_cap(j);
            if (partner[j] != 0) { ++j; continue; }
            idx_t xor_val = i ^ j;
            if (is_prime_fast(xor_val)) {
                partner[i] = (seq_t)j;
                partner[j] = (seq_t)i;
                break;
            }
            ++j;
        }
    }
    built_up_to = up_to;
}

/* Main sequence function ---------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;         /* undefined for n=0, but return 0 */
    /* Initialize prime sieve once with a generous upper bound. */
    if (prime_limit == 0) ensure_prime_limit(1 << 20);  /* ~1M enough for n<=10k */

    ensure_partner_cap(n);
    if (built_up_to < n) build_pairs_upto(n);

    seq_t sn = (seq_t) n;
    return sn ^ partner[n];
}
