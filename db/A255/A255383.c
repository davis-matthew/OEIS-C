/*
 * A255383  Compositorial mod sum-of-composites.
 * https://oeis.org/A255383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( ∏_{k=1..n} C_k ) mod ( Σ_{k=1..n} C_k ) where C_k is
 *            the k‑th composite number (4, 6, 8, 9, 10, …)
 * Range:     u8 n <= 16, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*--- global data ------------------------------------------------------*/
static bool *prime_flags = NULL;          /* sieve of Eratosthenes */
static size_t sieve_limit  = 0;
#define SIEVE_LIMIT 300000              /* sufficient for n≈10⁴ */

/* dynamic lists of composites and of their prefix sums */
static uint64_t *comps = NULL;           /* composite numbers */
static size_t comps_len = 0, comps_cap = 0;

static seq_t *pref  = NULL;              /* prefix sums as seq_t */
static size_t pref_cap = 0;

/*--- helpers -----------------------------------------------------------*/
static void init_sieve(void)
{
    if (prime_flags) return;              /* already initialized */
    prime_flags = (bool*)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!prime_flags) exit(EXIT_FAILURE);
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) prime_flags[i] = true;
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (prime_flags[i])
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                prime_flags[j] = false;
    sieve_limit = SIEVE_LIMIT;
}

/* ensure capacity for an array of uint64_t */
static void ensure_uint64_cap(uint64_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap == 0) ? 256 : (*cap * 2);
    while (new_cap < need) new_cap <<= 1;
    uint64_t *tmp = (uint64_t*)realloc(*arr, new_cap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    *arr = tmp;
    *cap = new_cap;
}

/* ensure capacity for an array of seq_t */
static void ensure_seq_cap(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap == 0) ? 256 : (*cap * 2);
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    *arr = tmp;
    *cap = new_cap;
}

/* generate composites until we have at least n of them */
static void fill_composites(size_t n)
{
    if (comps_len >= n) return;
    uint64_t cand = (comps_len == 0) ? 4 : comps[comps_len - 1] + 1;
    while (comps_len < n) {
        if (cand <= sieve_limit) {
            if (cand >= 4 && !prime_flags[cand]) {
                /* store composite */
                ensure_uint64_cap(&comps, &comps_cap, comps_len + 1);
                comps[comps_len] = cand;

                /* maintain prefix sum */
                seq_t prev = (comps_len == 0) ? 0 : pref[comps_len - 1];
                seq_t curr = prev + (seq_t)cand;
                ensure_seq_cap(&pref, &pref_cap, comps_len + 1);
                pref[comps_len] = curr;

                ++comps_len;
            }
        } else {
            /* should not happen with current SIEVE_LIMIT */
            exit(EXIT_FAILURE);
        }
        ++cand;
    }
}

/*--- required function -----------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined at offset 0 */
    init_sieve();
    fill_composites((size_t)n);

    seq_t sum = pref[(size_t)n - 1];      /* Σ of first n composites */
    seq_t prod_mod = (seq_t)1;
    for (size_t i = 0; i < (size_t)n; ++i) {
        prod_mod = (prod_mod * (seq_t)comps[i]) % sum;
    }
    return prod_mod;
}
