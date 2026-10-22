/*
 * A375637  Positive numbers k such that k! does not have nontrivial
 *          infinitary divisors that are factorials.
 * https://oeis.org/A375637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer k such that k! has no nontrivial
 *            infinitary divisor that is a factorial. The function below
 *            generates the sequence by a direct search
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Global data structures for primes, smallest prime factor and
   factorial prime exponents.  */
static int   *primes      = NULL;     /* list of primes, primes[0] == 2    */
static size_t primes_cnt = 0;
static int   *spf         = NULL;     /* smallest prime factor array */
static int   *prime_idx   = NULL;     /* prime -> index in primes[] */
static uint64_t *curr_exp = NULL;     /* current exponents for k!   */

/* Cache for the sequence itself.  */
static seq_t  *seq_cache = NULL;
static size_t seq_cap   = 0;
static size_t seq_len   = 0;

/* Current candidate k (the next integer to test).  */
static idx_t current_k = 1;          /* 1 is the first candidate */

/* Build prime tables up to LIMIT.  */

#define LIMIT 5000000UL   /* should be sufficient for n <= 10000   */

static void build_primes_and_spf(void)
{
    uint64_t n = LIMIT;
    primes      = (int *)malloc((n+1) * sizeof(int));
    spf         = (int *)malloc((n+1) * sizeof(int));
    prime_idx   = (int *)malloc((n+1) * sizeof(int));
    if (!primes || !spf || !prime_idx) abort();

    memset(spf, 0, (n+1) * sizeof(int));
    memset(prime_idx, -1, (n+1) * sizeof(int));

    size_t pc = 0;
    for (uint64_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = (int)i;
            primes[pc] = (int)i;
            prime_idx[i] = (int)pc;
            ++pc;
        }
        for (size_t j = 0; j < pc && (uint64_t)primes[j] <= spf[i] &&
             i * (uint64_t)primes[j] <= n; ++j) {
            spf[i * primes[j]] = primes[j];
        }
    }
    primes_cnt = pc;
    curr_exp = (uint64_t *)calloc(primes_cnt, sizeof(uint64_t));
    if (!curr_exp) abort();
}

/* Legendre's formula: exponent of prime p in n!  */
static uint64_t legendre_exp(idx_t n, int p)
{
    uint64_t exp = 0;
    uint64_t t = n;
    while (t) {
        t /= p;
        exp += t;
    }
    return exp;
}

/* Add prime factorisation of x to the current exponent array curr_exp
   (used for building factorial exponents incrementally).  */
static void add_factorisation(idx_t x)
{
    while (x > 1) {
        int p = spf[x];
        int idx = prime_idx[p];
        int cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
        }
        curr_exp[idx] += cnt;
    }
}

/* Test whether k! has a nontrivial infinitary divisor that is a factorial.
   Returns 1 if such a divisor exists, 0 otherwise.  */
static int has_infinitary_factorial_divisor(idx_t k)
{
    if (k <= 2) return 0;  /* 1! and 2! are always OK */

    /* List of primes up to k/2.  These are the only primes that matter,
       because for primes p > k/2 the exponent in k! is 1 and any m < k
       has exponent 0, which satisfies the submask condition automatically. */
    size_t end_idx = 0;
    uint64_t half = k >> 1;
    while (end_idx < primes_cnt && (uint64_t)primes[end_idx] <= half)
        ++end_idx;

    /* Exponent of 2 in k! is stored in curr_exp[0].  */
    uint64_t e_k2 = curr_exp[0];

    /* Try every possible m from k-1 down to 2.  */
    for (idx_t m = k - 1; m > 1; --m) {
        /* Prime 2 check.  Exponent of 2 in m! = m - popcount(m).  */
        uint64_t e_m2 = (uint64_t)m - (uint64_t)__builtin_popcountll(m);
        if ((e_m2 & ~e_k2) != 0)
            continue;   /* fails on prime 2 -> not a divisor */

        /* Check other primes ( <= k/2 ).  */
        int ok = 1;
        for (size_t pi = 1; pi < end_idx; ++pi) {
            int p = primes[pi];
            uint64_t e_m = legendre_exp(m, p);
            if ((e_m & ~curr_exp[pi]) != 0) {
                ok = 0;
                break;  /* fails on this prime */
            }
        }
        if (ok)
            return 1;  /* found a nontrivial factorial divisor */
    }
    return 0;  /* no such divisor found */
}

/* Extend the sequence cache up to at least index n.  */
static void ensure_sequence(idx_t n)
{
    if (n <= seq_len) return;

    /* Make sure prime tables exist.  */
    if (!primes) build_primes_and_spf();

    /* Grow the sequence array as needed.  */
    while (seq_len < n) {
        /* Update factorial exponents by adding current_k.  */
        add_factorisation(current_k);

        if (!has_infinitary_factorial_divisor(current_k)) {
            /* Allocate more space if necessary.  */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 1;
                seq_cache = (seq_t *)realloc(seq_cache,
                                            seq_cap * sizeof(seq_t));
                if (!seq_cache) abort();
            }
            seq_cache[seq_len++] = (seq_t)current_k;
        }
        ++current_k;
    }
}

/* The required function.  */
seq_t a(idx_t n)
{
    ensure_sequence(n);
    return seq_cache[n - 1];
}
