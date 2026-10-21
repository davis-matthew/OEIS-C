/*
 * A257474  Number of unordered ways to write n = a + b, where a and b are
 *          distinct elements of the set {floor(x/3): 3*x-1 and 3*x+1 are twin
 *          prime}, one of a and b is even, and one of a and b has the form
 *          p-1 or p-2 with p prime.
 * https://oeis.org/A257474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9970, u16 n <= 9970, u32 n <= 9970, u64 n <= 9970,
 *            u128 n <= 9970, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A257474 – Number of unordered ways to write n = a + b where
   a,b are distinct elements of { floor(x/3) | 3x-1 and 3x+1 are twin primes },
   one of a,b is even, and one of a,b is of the form p-1 or p-2 with p prime. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* ---------- Prime sieve utilities ---------- */
static bool *prime_arr = NULL;
static size_t prime_lim = 0;

static void sieve_upto(size_t lim) {
    if (lim <= prime_lim) return;
    bool *new_arr = (bool*)malloc((lim + 1) * sizeof(bool));
    if (!new_arr) exit(EXIT_FAILURE);
    for (size_t i = 0; i <= lim; ++i) new_arr[i] = true;
    new_arr[0] = new_arr[1] = false;
    for (size_t i = 2; i * i <= lim; ++i) {
        if (new_arr[i]) {
            for (size_t j = i * i; j <= lim; j += i) new_arr[j] = false;
        }
    }
    free(prime_arr);
    prime_arr = new_arr;
    prime_lim = lim;
}

/* ---------- Cached data structures ---------- */
static idx_t *S_arr       = NULL;   /* sorted unique values of floor(x/3)   */
static size_t S_len       = 0;
static size_t S_cap       = 0;

static bool *S_in_arr     = NULL;   /* S_in_arr[v] == true iff v in set S   */
static bool *goodPrimeArr = NULL;   /* goodPrimeArr[v] == true iff v+1 or v+2 prime */
static size_t S_cap_arr   = 0;

static size_t max_n_seen  = 0;      /* largest n for which data is prepared   */
static size_t max_x_seen  = 0;      /* largest x processed                    */

/* Ensure the boolean arrays are large enough for indices up to n. */
static void ensure_bool_arrays(size_t n) {
    if (n <= S_cap_arr) return;
    size_t new_cap = n + 5;
    bool *tmp1 = (bool*)realloc(S_in_arr, (new_cap + 1) * sizeof(bool));
    bool *tmp2 = (bool*)realloc(goodPrimeArr, (new_cap + 1) * sizeof(bool));
    if (!tmp1 || !tmp2) exit(EXIT_FAILURE);
    memset(tmp1 + S_cap_arr + 1, 0, (new_cap - S_cap_arr) * sizeof(bool));
    memset(tmp2 + S_cap_arr + 1, 0, (new_cap - S_cap_arr) * sizeof(bool));
    S_in_arr = tmp1;
    goodPrimeArr = tmp2;
    S_cap_arr = new_cap;
}

/* Expand cached data to handle values up to n. */
static void expand_to(size_t n) {
    /* determine necessary bounds for prime sieve */
    size_t limit = 9 * n + 10;           /* covers 3x+1 for x up to 3n+2 */
    sieve_upto(limit);
    ensure_bool_arrays(n);

    size_t new_x_max = 3 * n + 2;   /* we need floor(x/3) up to n   */
    for (size_t x = max_x_seen + 1; x <= new_x_max; ++x) {
        size_t p1 = 3 * x - 1;      /* candidate twin primes    */
        size_t p2 = p1 + 2;
        if (p1 <= prime_lim && p2 <= prime_lim && prime_arr[p1] && prime_arr[p2]) {
            size_t val = x / 3;
            if (!S_in_arr[val]) {
                /* append val to sorted array S_arr */
                if (S_len == S_cap) {
                    size_t new_cap = S_cap ? S_cap * 2 : 8;
                    idx_t *tmp = (idx_t*)realloc(S_arr, new_cap * sizeof(idx_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    S_arr = tmp;
                    S_cap = new_cap;
                }
                S_arr[S_len++] = (idx_t)val;
                S_in_arr[val] = true;
                goodPrimeArr[val] = prime_arr[val + 1] || prime_arr[val + 2];
            }
        }
    }
    max_x_seen = new_x_max;
    max_n_seen = n;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for n<1, safe guard */

    size_t sn = (size_t)n;
    if (sn > max_n_seen) expand_to(sn);

    seq_t result = 0;
    size_t half = sn / 2;

    for (size_t i = 0; i < S_len; ++i) {
        idx_t a_val = S_arr[i];
        if ((size_t)a_val > half) break;
        idx_t b_val = (idx_t)(sn - a_val);
        if (b_val == a_val) continue;           /* distinctness required */
        if (b_val > (idx_t)(max_n_seen)) continue;
        if (!S_in_arr[(size_t)b_val]) continue;   /* b not in set S         */

        bool even_cond  = ((a_val & 1) == 0) || ((b_val & 1) == 0);
        bool prime_cond = goodPrimeArr[(size_t)a_val] || goodPrimeArr[(size_t)b_val];
        if (even_cond && prime_cond) ++result;
    }
    return result;
}
