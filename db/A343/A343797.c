/*
 * A343797  Numbers k = p + q (p,q prime), where k | (p*q +- 1) for more than
 *          1 pair, (p,q).
 * https://oeis.org/A343797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A343797
 *  Numbers k = p + q (p,q prime), where k | (p*q ± 1) for more than one pair (p,q).
 *  Computed by a straightforward search: for each integer k, all primes
 *  p ≤ k/2 are considered; if p and q=k-p are both prime and
 *  p^2 ≡ ±1 (mod k), we count a pair.  When two such pairs are found,
 *  k is appended to the sequence.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- Prime sieve data --------------------------------------- */
static uint8_t *is_prime = NULL;          /* 0 = prime, 1 = composite   */
static idx_t    sieve_limit = 0;          /* current largest sieved n    */
static uint64_t *prime_list = NULL;       /* list of primes up to limit  */
static size_t   prime_cnt = 0;            /* number of primes stored     */

/* --- Sequence cache data ------------------------------------ */
static seq_t *seq_cache = NULL;           /* stored sequence values       */
static size_t seq_size  = 0;              /* number of values stored      */
static size_t seq_cap   = 0;              /* allocated capacity of cache  */

/* --- Last integer examined by the search --------------------- */
static idx_t last_checked = 2;            /* next k to test is last_checked+1 */

/* --- Forward declarations ----------------------------------- */
static void ensure_sieve(idx_t limit);
static bool check_k(idx_t k);
static void grow_seq(size_t need);

/* --- Ensure we have a prime sieve up to 'limit' (inclusive) --- */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;

    /* Allocate new sieve array */
    uint8_t *new_sieve = (uint8_t *)calloc((size_t)limit + 1, sizeof(uint8_t));
    if (!new_sieve) exit(1);

    /* Sieve of Eratosthenes: 0 and 1 are composite */
    new_sieve[0] = new_sieve[1] = 1;
    for (idx_t p = 2; p <= limit / p; ++p) {
        if (!new_sieve[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                new_sieve[m] = 1;
        }
    }

    /* Build prime list */
    size_t capacity = (size_t)(limit / 2) + 1;
    uint64_t *new_list = (uint64_t *)realloc(prime_list,
                                             capacity * sizeof(uint64_t));
    if (!new_list) exit(1);
    prime_list = new_list;
    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!new_sieve[i]) prime_list[prime_cnt++] = (uint64_t)i;
    }

    /* Replace old sieve */
    free(is_prime);
    is_prime = new_sieve;
    sieve_limit = limit;
}

/* --- Grow sequence cache as needed --------------------------- */
static void grow_seq(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 256;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_seq = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_seq) exit(1);
    seq_cache = new_seq;
    seq_cap = new_cap;
}

/* --- Test whether k satisfies the definition ----------------- */
static bool check_k(idx_t k)
{
    size_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > k / 2) break;                 /* ensure p ≤ q */

        uint64_t q = k - p;
        if (q < p) continue;                  /* duplicate ordering */
        if (is_prime[q]) continue;             /* q not prime */

        /* condition: p^2 ≡ ±1 (mod k)  (equivalent to k | p*q ± 1) */
        unsigned __int128 sq = (unsigned __int128)p * p;
        idx_t rem = (idx_t)(sq % k);
        if (rem == 1 || rem == k - 1) {
            ++count;
            if (count >= 2) return true;
        }
    }
    return false;
}

/* --- Generate sequence values up to n_needed (1‑based) ------- */
static void generate_until(idx_t n_needed)
{
    if (seq_size >= n_needed) return;
    grow_seq(n_needed);

    if (!is_prime) ensure_sieve(64);          /* initial sieve */

    idx_t k = last_checked + 1;
    for (; seq_size < n_needed; ++k) {
        if (k > sieve_limit) ensure_sieve(k * 2 + 100);
        if (check_k(k)) {
            seq_cache[seq_size++] = (seq_t)k;
        }
    }
    last_checked = k - 1;
}

/* --- Public sequence routine -------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for index 0 */
    generate_until(n);
    return seq_cache[n - 1];
}
