/*
 * A265398  Perform one x^2 -> x+1 reduction for the polynomial with
 *          nonnegative integer coefficients that is encoded in the prime
 *          factorization of n.
 * https://oeis.org/A265398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is completely multiplicative: a(1)=1, a(2)=2, a(3)=3,
 *            a(prime_k)=prime_{k-1}*prime_{k-2} for k>2
 * Range:     u8 n <= 22, u16 n <= 268, u32 n <= 9930, u64 n <= 9930,
 *            u128 n <= 9930, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


/* ---------- Prime sieve and helpers ---------- */
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t     prime_count = 0;    /* number of primes stored */
static size_t     prime_cap    = 0;    /* allocated size of prime_list */

/* Build all primes <= limit and store them in prime_list. */
static void build_primes_upto(size_t limit)
{
    /* free previously allocated array if needed */
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }

    /* simple sieve of Eratosthenes */
    bool *sieve = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!sieve) return;          /* allocation failure – abort silently */
    for (size_t i = 2; i <= limit; ++i) sieve[i] = true;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                sieve[j] = false;
        }
    }

    /* count primes to allocate array */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) ++cnt;

    prime_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) { free(sieve); return; }
    prime_count = cnt;
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) prime_list[idx++] = (uint64_t)i;

    free(sieve);
}

/* Ensure primes up to the given limit are available. */
static void ensure_primes_upto(size_t limit)
{
    if (prime_list == NULL || limit > prime_list[prime_count - 1])
        build_primes_upto(limit);
}

/* Binary search for the 1‑based rank of a prime p in prime_list. */
static size_t prime_rank(uint64_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p) return mid + 1;
        if (prime_list[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return 0;   /* should not happen for a prime input */
}

/* Raise a base to a non‑negative integer exponent, using seq_t arithmetic. */
static seq_t pow_seq(seq_t base, unsigned int exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* ---------- Factorisation helper ---------- */
static void factor_uint64(uint64_t n, uint64_t *pf, uint64_t *exp, size_t *cnt)
{
    size_t r = 0;
    uint64_t x = n;
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= x; ++i) {
        uint64_t p = prime_list[i];
        if (x % p == 0) {
            uint64_t e = 0;
            do { x /= p; ++e; } while (x % p == 0);
            pf[r] = p; exp[r] = e; ++r;
        }
    }
    if (x > 1) { pf[r] = x; exp[r] = 1; ++r; }
    *cnt = r;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t un = (uint64_t)n;
    /* need primes up to un to factor and obtain ranks */
    ensure_primes_upto(un);

    /* temporary storage for factors; 64 is generous for n up to 10^18 */
    uint64_t pf[64], exp[64];
    size_t fac_cnt;
    factor_uint64(un, pf, exp, &fac_cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p = pf[i];
        unsigned int e = (unsigned int)exp[i];

        seq_t ap;
        if (p == 2)          ap = (seq_t)2;
        else if (p == 3)     ap = (seq_t)3;
        else {
            size_t rk = prime_rank(p);          /* 1‑based rank */
            /* rk >= 3 for p >= 5 */
            seq_t prev1 = (seq_t)prime_list[rk - 2]; /* prime(k-1) */
            seq_t prev2 = (seq_t)prime_list[rk - 3]; /* prime(k-2) */
            ap = prev1 * prev2;
        }
        seq_t ap_pow = pow_seq(ap, e);
        result *= ap_pow;
    }
    return result;
}
