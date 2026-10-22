/*
 * A306877  Lexicographically earliest sequence of distinct positive terms
 *          such that, for any n > 0, among the prime divisors of n * a(n)
 *          there are at least 4 runs of consecutive prime numbers.
 * https://oeis.org/A306877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9868, u32 n <= 9868, u64 n <= 9868,
 *            u128 n <= 9868, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest self‑inverse permutation of the natural
   numbers for which, for every n>0, the set of distinct prime divisors of
   n*a(n) contains at least four runs of consecutive primes (prime
   numbers).  The function returns the n‑th term of that sequence. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/next_prime.h"   /* for uint64_t next_prime(uint64_t) */

 /* --- prime sieve ------------------------------------------------------ */

#define PRIME_LIMIT 4000000   /* enough for all products up to 1e13 */

static uint32_t *prime_list   = NULL;
static size_t   prime_cnt     = 0;

/* initialise prime_list with all primes up to PRIME_LIMIT */
static void init_primes(void)
{
    if (prime_cnt > 0) return;       /* already done */

    bool *is_comp = (bool*)calloc(PRIME_LIMIT + 1, sizeof(*is_comp));
    if (!is_comp) abort();

    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            /* store prime */
            size_t new_cnt = prime_cnt + 1;
            uint32_t *tmp = (uint32_t*)realloc(prime_list,
                                               new_cnt * sizeof(*prime_list));
            if (!tmp) { free(is_comp); abort(); }
            prime_list = tmp;
            prime_list[prime_cnt++] = (uint32_t)i;

            if (i * i <= PRIME_LIMIT) {
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
}

/* --- structure for the permutation ----------------------------------- */

static seq_t *perm   = NULL;   /* perm[0] unused, perm[i] = a(i) */
static size_t perm_cap = 0;    /* number of allocated slots */

/* ensure that indices up to 'idx' are reachable in perm */
static void ensure_capacity(idx_t idx)
{
    if ((size_t)idx < perm_cap) return;   /* already large enough */

    size_t new_cap = perm_cap ? perm_cap << 1 : 8;
    while ((size_t)idx >= new_cap) new_cap <<= 1;

    seq_t *new_arr = (seq_t*)realloc(perm, new_cap * sizeof(*perm));
    if (!new_arr) abort();

    memset(new_arr + perm_cap, 0, (new_cap - perm_cap) * sizeof(*perm));
    perm = new_arr;
    perm_cap = new_cap;
}

/* --- test whether n*m satisfies the run condition --------------------- */
static bool ok_runs(idx_t n, idx_t m)
{
    unsigned __int128 prod128 = (unsigned __int128)n * (unsigned __int128)m;
    if (prod128 > (uint64_t)-1)   /* too large for 64‑bit factorisation */
        return false;
    uint64_t prod = (uint64_t)prod128;

    /* factorise prod into distinct primes */
    uint64_t tmp = prod;
    uint64_t primes[64];
    size_t   pc   = 0;

    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] *
                                 (uint64_t)prime_list[i] <= tmp; ++i) {
        uint32_t p = prime_list[i];
        if (tmp % p == 0) {
            primes[pc++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) primes[pc++] = tmp;

    if (pc == 0) return false;      /* product 1, no primes */

    /* count runs of consecutive primes */
    size_t runs = 1;
    for (size_t i = 1; i < pc; ++i) {
        uint64_t nextp = next_prime(primes[i-1]); /* next prime after primes[i-1] */
        if (nextp != primes[i]) runs++;
    }
    return runs >= 4;
}

/* --- build the permutation up to index 'limit' ------------------------ */
static void build_up_to(idx_t limit)
{
    static idx_t built = 0;
    if (built >= limit) return;

    ensure_capacity(limit);

    for (idx_t n = built + 1; n <= limit; ++n) {
        if (perm[n] != 0) continue;   /* already matched */

        /* search for the smallest free m such that both n and m are free
           and the run condition holds */
        for (idx_t m = n; ; ++m) {
            ensure_capacity(m);
            if (perm[m] != 0) continue;

            if (ok_runs(n, m)) {
                perm[n] = m;
                perm[m] = n;
                break;
            }
        }
    }
    built = limit;
}

/* --- public routine --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, safe default */

    if (perm == NULL) {
        perm_cap = 8;
        perm = (seq_t*)calloc(perm_cap, sizeof(*perm));   /* perm[0] unused */
        if (!perm) abort();
    }

    init_primes();                         /* prime list once */
    build_up_to(n);                        /* ensure all values up to n */
    return perm[n];
}
