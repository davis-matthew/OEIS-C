/*
 * A349622  Numbers k for which 2k-1 can be obtained with successive prime
 *          shifts towards larger primes (by iterating A003961, starting from
 *          k).
 * https://oeis.org/A349622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A349622:  Numbers k for which 2k‑1 can be obtained by successive prime
 *           shifts (A003961) starting from k.  The function a(idx_t n)
 *           returns the nth term (offset 1) by simulating the prime‑shift
 *           process and collecting the qualifying k's.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Prime utilities – the build supplies seq_t and idx_t */
static seq_t *primes = NULL;
static size_t primes_cnt = 0;

/* Sieve limit – sufficient for k up to several thousand terms */
#define SIEVE_MAX 2000000ULL

/* --------------------------------------------------------------------- */
/* Initialise the sieve of Eratosthenes and store primes in an array.      */
/* --------------------------------------------------------------------- */
static void init_primes(void)
{
    if (primes != NULL)
        return;                         /* already initialised */

    /* Allocate a bit sieve – true = composite */
    size_t sieve_size = (size_t)(SIEVE_MAX + 1);
    bool *sieve = (bool *)calloc(sieve_size, sizeof(bool));
    if (!sieve) abort();

    for (size_t i = 2; i * i <= SIEVE_MAX; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= SIEVE_MAX; j += i)
                sieve[j] = true;
        }
    }

    /* Count primes */
    primes_cnt = 0;
    for (size_t i = 2; i <= SIEVE_MAX; ++i)
        if (!sieve[i]) ++primes_cnt;

    /* Allocate and fill array */
    primes = (seq_t *)malloc(primes_cnt * sizeof(seq_t));
    if (!primes) abort();

    size_t idx = 0;
    for (size_t i = 2; i <= SIEVE_MAX; ++i)
        if (!sieve[i]) primes[idx++] = (seq_t)i;

    free(sieve);
}

/* --------------------------------------------------------------------- */
/* Return the next prime strictly larger than x.                          */
/* --------------------------------------------------------------------- */
static seq_t next_prime_after(seq_t x)
{
    if (x < 2) return 2;

    /* If x is beyond the sieve, use simple trial division by the stored primes. */
    if ((idx_t)x > SIEVE_MAX) {
        seq_t y = x + 1;
        for (;;) {
            bool prime = true;
            for (size_t i = 0; i < primes_cnt; ++i) {
                seq_t p = primes[i];
                if (p * p > y) break;
                if (y % p == 0) { prime = false; break; }
            }
            if (prime) return y;
            ++y;
        }
    }

    /* Binary search in the pre‑computed prime array. */
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (primes[mid] > x) hi = mid;
        else                 lo = mid + 1;
    }
    return (lo < primes_cnt) ? primes[lo] : next_prime_after(x);
}

/* --------------------------------------------------------------------- */
/* Prime shift function g(n) – A003961.                                  */
/* --------------------------------------------------------------------- */
static seq_t prime_shift(seq_t n)
{
    if (n <= 1) return 1;

    seq_t result = 1;
    seq_t tmp = n;

    for (size_t i = 0; i < primes_cnt; ++i) {
        seq_t p = primes[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            unsigned exp = 0;
            while (tmp % p == 0) { tmp /= p; ++exp; }
            seq_t q = next_prime_after(p);
            while (exp--) result *= q;
        }
    }

    if (tmp > 1) {             /* remaining prime factor */
        seq_t q = next_prime_after(tmp);
        result *= q;
    }
    return result;
}

/* --------------------------------------------------------------------- */
/* Check if a given k satisfies the sequence condition.                  */
/* --------------------------------------------------------------------- */
static bool qualifies(idx_t k)
{
    seq_t target = ((seq_t)k) * 2ULL - 1ULL;
    seq_t cur    = ((seq_t)k);

    while (cur < target) {
        cur = prime_shift(cur);
        if (cur == target) return true;
        if (cur > target) return false;
    }
    return cur == target;
}

/* --------------------------------------------------------------------- */
/* Storage for computed terms.                                            */
/* --------------------------------------------------------------------- */
static seq_t *seq_arr = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

static void ensure_seq_capacity(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = (seq_cap == 0) ? 8 : seq_cap << 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Next k to test – keeps state between calls.                           */
static idx_t next_k_to_test = 1;

/* --------------------------------------------------------------------- */
/* Public function: return the nth term of A349622 (offset 1).           */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined for offset < 1 */

    init_primes();

    /* Generate terms until at least n are available. */
    while (seq_len < n) {
        if (qualifies(next_k_to_test)) {
            ensure_seq_capacity(seq_len + 1);
            seq_arr[seq_len++] = (seq_t)next_k_to_test;
        }
        ++next_k_to_test;
    }

    return seq_arr[n - 1];
}
