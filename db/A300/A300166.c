/*
 * A300166  Numbers of the form m^2 + 1 that can be expressed in more than one
 *          way as j^2 + k^2 with j > k > 1 and gcd(j,k) = 1.
 * https://oeis.org/A300166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 38, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A300166 – numbers of the form m^2+1 that have at least two
 *  primitive representations as j^2 + k^2 with j > k > 1 and gcd(j,k)=1.
 *
 *  The algorithm generates terms in order: for increasing m compute
 *  n = m^2+1, factor n and count distinct prime factors ≡ 1 (mod 4).
 *  If that count is ≥ 3 the number has at least 4 primitive
 *  representations, one of which is j=m, k=1; therefore at least
 *  two other representations have j>k>1.  Such n are stored and
 *  the nth stored value is returned by a(idx_t n).  The implementation
 *  uses simple trial division with a pre‑computed list of primes up to
 *  5 000 000, sufficient for the first 10 000 terms (m < 10^6, n < 10^12).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- prime sieve ------------------------------------ */
static uint32_t *primes = NULL;   /* array of primes ≤ 5_000_000      */
static size_t   prime_cnt = 0;    /* number of primes                */
static bool     primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;

    /*--- generate primes up to LIMIT using a simple sieve ---*/
    const unsigned int LIMIT = 5000000U;
    bool *is_comp = (bool *)calloc((size_t)LIMIT + 1, sizeof(bool));
    if (!is_comp) abort();

    /* approximate number of primes ≤ LIMIT (p(n) ~ n / log n) */
    size_t est = LIMIT / 10 + 1000;          /* safe upper bound   */
    primes = (uint32_t *)malloc(est * sizeof(uint32_t));
    if (!primes) abort();

    for (unsigned int i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            if (prime_cnt == est) {   /* enlarge buffer if necessary */
                est *= 2;
                uint32_t *tmp = (uint32_t *)realloc(primes, est * sizeof(uint32_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (unsigned int j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
    primes_inited = true;
}

/* ---------- helper: count distinct p ≡ 1 (mod 4) in n --------*/
static int count_distinct_1mod4(uint64_t n)
{
    int cnt = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            if (p % 4 == 1) ++cnt;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1 && tmp % 4 == 1) ++cnt;
    return cnt;
}

/* ---------- storage for generated terms ---------------------*/
static seq_t *terms = NULL;
static size_t term_len = 0;   /* number of stored terms     */
static size_t term_cap = 0;   /* allocated capacity         */
static idx_t  last_m  = 0;    /* last m processed           */

static void ensure_term_capacity(size_t needed)
{
    if (term_cap >= needed) return;
    size_t new_cap = term_cap ? term_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    terms = tmp;
    term_cap = new_cap;
}

static void generate_terms(idx_t need)
{
    if ((idx_t)term_len >= need) return;

    init_primes();
    ensure_term_capacity((size_t)need);

    while ((idx_t)term_len < need) {
        ++last_m;
        uint64_t m64 = (uint64_t)last_m;          /* for arithmetic       */
        seq_t n_val = (seq_t)m64 * m64 + (seq_t)1;
        uint64_t n64  = m64 * m64 + 1ULL;         /* for factorisation   */

        if (count_distinct_1mod4(n64) >= 3) {
            terms[term_len++] = n_val;
        }
    }
}

/* ---------- public sequence function ------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    generate_terms(n);
    return terms[n - 1];
}
