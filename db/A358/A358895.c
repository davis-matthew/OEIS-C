/*
 * A358895  Numbers k such that p(k)^p(k + 1) < p(k + 2)^p(k), where p(k) =
 *          prime(k).
 * https://oeis.org/A358895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A358895
 * Formula: a(n) = the n-th integer k such that p(k)^p(k+1) < p(k+2)^p(k),
 * where p(k) is the k‑th prime (offset 1).  The routine keeps a list of
 * primes and a list of qualifying k’s, extending them lazily as needed.
 * It uses logarithms to compare the huge powers without overflow. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* ----- prime storage --------------------------------------------------- */
static uint64_t *primes = NULL;            /* primes[0] = 2 = p(1) */
static size_t primes_len  = 0;
static size_t primes_cap  = 0;
static uint64_t prime_candidate = 3;       /* next odd candidate */

/* grow the prime array to at least new_cap elements */
static void grow_primes(size_t new_cap) {
    size_t cap = primes_cap;
    if (cap == 0) cap = 16;
    while (cap < new_cap) cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(primes, cap * sizeof(uint64_t));
    if (!tmp) abort();                     /* out of memory */
    primes = tmp;
    primes_cap = cap;
}

/* add a prime to the array, growing it if necessary */
static void add_prime(uint64_t p) {
    if (primes_len == primes_cap) grow_primes(primes_len + 1);
    primes[primes_len++] = p;
}

/* generate the next prime and store it */
static void generate_next_prime(void) {
    while (true) {                /* search until a prime is found */
        bool is_prime = true;
        uint64_t limit = (uint64_t)sqrtl((long double)prime_candidate);
        for (size_t i = 0; i < primes_len && primes[i] <= limit; ++i) {
            if (prime_candidate % primes[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            add_prime(prime_candidate);
            prime_candidate += 2;   /* skip even numbers */
            return;
        }
        prime_candidate += 2;
    }
}

/* ensure that the prime array holds at least (idx+1) primes (0‑based) */
static void ensure_primes_upto_idx(size_t idx) {
    if (primes_len == 0) {          /* initialise the first prime */
        add_prime(2);
        prime_candidate = 3;
    }
    while (primes_len <= idx) generate_next_prime();
}

/* ----- k‑list storage -------------------------------------------------- */
static idx_t *klist = NULL;           /* stored qualifying k values (1‑based) */
static size_t klen  = 0;
static size_t kcap  = 0;
static idx_t next_k = 1;              /* next k to test (1‑based) */

/* grow the klist to at least new_cap elements */
static void grow_klist(size_t new_cap) {
    size_t cap = kcap;
    if (cap == 0) cap = 16;
    while (cap < new_cap) cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(klist, cap * sizeof(idx_t));
    if (!tmp) abort();
    klist = tmp;
    kcap = cap;
}

/* append a qualifying k to the list */
static void append_k(idx_t k) {
    if (klen == kcap) grow_klist(klen + 1);
    klist[klen++] = k;
}

/* test whether a given k satisfies the inequality */
static bool k_is_candidate(idx_t k) {
    /* need primes up to p(k+2) → index k+1 (0‑based) */
    ensure_primes_upto_idx((size_t)(k + 1));
    /* fetch the three consecutive primes */
    uint64_t pk   = primes[k - 1];
    uint64_t pk1  = primes[k];
    uint64_t pk2  = primes[k + 1];
    long double left  = (long double)pk1 * logl((long double)pk);
    long double right = (long double)pk   * logl((long double)pk2);
    return left < right;
}

/* extend the k list until it contains at least n entries */
static void ensure_klist(size_t n) {
    while (klen < n) {
        if (k_is_candidate(next_k)) append_k(next_k);
        ++next_k;
    }
}

/* ----- public sequence function --------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;              /* undefined for n < 1 */
    ensure_klist((size_t)n);
    return (seq_t)klist[(size_t)(n - 1)];
}
