/*
 * A371168  Positive integers with fewer prime factors (A001222) than distinct
 *          divisors of prime indices (A370820).
 * https://oeis.org/A371168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer m such that * Ω(m) < D, where
 *            Ω(m) is the number of prime factors of m * counted with
 *            multiplicity (A001222) and D is the number of * distinct
 *            divisors of the multiset of prime indices of m * (A370820). The
 *            sequence is generated on demand. * (See OEIS A371168.)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define LIMIT_N     2000000U      /* sieve limit (must be > expected terms) */
#define MAX_PRIMES  200000U       /* ≥ number of primes <= LIMIT_N (≈ 148933) */

static uint32_t spf[LIMIT_N + 1];
static uint32_t index_of[LIMIT_N + 1];
static uint32_t primes[MAX_PRIMES];
static size_t prime_count = 0;

static int visit_stamp[LIMIT_N + 1];
static int cur_stamp = 0;

static void init_sieve(void)
{
    static bool initialized = false;
    if (initialized)
        return;
    initialized = true;

    for (uint32_t i = 2; i <= LIMIT_N; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (prime_count < MAX_PRIMES) {
                primes[prime_count] = i;
                index_of[i] = (uint32_t)(prime_count + 1); /* 1‑based index */
                ++prime_count;
            }
        }
        for (size_t j = 0; j < prime_count && primes[j] <= spf[i] && (uint64_t)i * primes[j] <= LIMIT_N; ++j) {
            uint32_t v = i * primes[j];
            spf[v] = primes[j];
        }
    }
    /* index_of[prime] already set; others remain 0 */
    /* visit_stamp is zero-initialized by static declaration */
}

/* generate all divisors of small integer idx (idx <= LIMIT_N) */
static void gen_divisors(uint32_t idx, int *divs, int *cnt)
{
    if (idx == 1) {
        divs[0] = 1;
        *cnt = 1;
        return;
    }
    uint32_t tmp = idx;
    int fac[10];
    int exp[10];
    int sz = 0;
    while (tmp > 1) {
        uint32_t p = spf[tmp];
        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        fac[sz] = (int)p;
        exp[sz] = e;
        ++sz;
    }

    divs[0] = 1;
    int size = 1;
    for (int i = 0; i < sz; ++i) {
        int curp = fac[i];
        int curexp = exp[i];
        int curSize = size;
        int mul = 1;
        for (int e = 1; e <= curexp; ++e) {
            mul *= curp;
            for (int j = 0; j < curSize; ++j) {
                divs[size++] = divs[j] * mul;
            }
        }
    }
    *cnt = size;
}

/* check if a positive integer n satisfies the condition */
static bool cond_satisfies(uint64_t n)
{
    int omega = 0;        /* number of prime factors with multiplicity */
    int distinct_divs = 0;/* number of distinct divisors of prime indices */

    uint64_t tmp = n;
    while (tmp > 1) {
        uint32_t p = (tmp <= LIMIT_N) ? spf[(uint32_t)tmp] : 0;
        if (p == 0) {          /* prime larger than our sieve limit */
            p = (uint32_t)tmp;
        }
        int cnt = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++cnt;
            ++omega;
        }

        uint32_t idx = (p <= LIMIT_N) ? index_of[p] : 0;
        if (idx == 0)
            continue; /* should not happen for p <= LIMIT_N */

        int divs[128];
        int dcnt;
        gen_divisors(idx, divs, &dcnt);
        for (int i = 0; i < dcnt; ++i) {
            uint32_t d = (uint32_t)divs[i];
            if (visit_stamp[d] != cur_stamp) {
                visit_stamp[d] = cur_stamp;
                ++distinct_divs;
            }
        }
    }
    return omega < distinct_divs;
}

/* dynamic array to store sequence terms */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static uint64_t last_candidate = 2; /* last n processed */

static void grow_terms(size_t min_cap)
{
    size_t new_cap = (terms_cap == 0) ? 64 : terms_cap;
    while (new_cap < min_cap)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_arr) { /* out of memory */
        abort();
    }
    terms = new_arr;
    terms_cap = new_cap;
}

static void append_term(seq_t v)
{
    if (terms_len == terms_cap)
        grow_terms(terms_len + 1);
    terms[terms_len++] = v;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_sieve();

    while (terms_len < (size_t)n) {
        ++last_candidate;
        ++cur_stamp;
        if (cond_satisfies(last_candidate)) {
            append_term((seq_t)last_candidate);
        }
    }
    return terms[n - 1];
}
