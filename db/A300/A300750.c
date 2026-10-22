/*
 * A300750  a(n) is the least positive k such that prime(n) divides
 *          (prime(n+1)*prime(n+2)*...*prime(n+k))+1.
 * https://oeis.org/A300750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A300750
    a(n) = least positive k such that prime(n) divides
    (prime(n+1)·prime(n+2)·…·prime(n+k)) + 1
    (offset = 1).  The value is found by iteratively
    multiplying the next primes modulo prime(n) until the modulus
    condition is satisfied.  The implementation grows a
    static prime list as needed. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* --------------------------------------------------------------------
   Helper data structures: a dynamic array of primes.
   -------------------------------------------------------------------- */
static uint64_t *prime_list     = NULL; /* 1‑based, prime_list[0] = 2 */
static size_t   prime_count    = 0;     /* number of primes currently stored */
static size_t   prime_capacity = 0;     /* allocated capacity */

/* Grow the prime array to at least `new_cap` entries. */
static void grow_prime_array(size_t new_cap)
{
    if (new_cap <= prime_capacity) return;
    size_t cap = (prime_capacity == 0) ? 16 : prime_capacity * 2;
    while (cap < new_cap) cap *= 2;
    void *tmp = realloc(prime_list, cap * sizeof(*prime_list));
    if (!tmp) abort();                /* out of memory */
    prime_list     = (uint64_t*)tmp;
    prime_capacity = cap;
}

/* Simple trial division using the primes already known.  */
static bool is_prime_candidate(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Ensure that at least `k` primes are available.  k is 1‑based. */
static void ensure_primes_idx(idx_t k)
{
    while (prime_count < (size_t)k) {
        uint64_t cand = (prime_count == 0) ? 2 : prime_list[prime_count - 1] + 1;
        while (!is_prime_candidate(cand)) ++cand;
        grow_prime_array(prime_count + 1);
        prime_list[prime_count++] = cand;
    }
}

/* Retrieve the k‑th prime (1‑based). */
static uint64_t get_prime(idx_t k)
{
    ensure_primes_idx(k);
    return prime_list[k - 1];
}

/* --------------------------------------------------------------------
   The sequence function.  Return 0 for n < 1 (undefined in the OEIS)
   but the function is otherwise well‑defined for any n >= 1 and
   arbitrary call order.  The computation is O(a(n)) primes generated
   after prime(n).  This is fast enough for n up to at least 10 000.
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* undefined for n=0 */

    uint64_t target = get_prime(n);           /* prime(n) */
    seq_t   mod = (seq_t)target;
    seq_t   prod = 1;                          /* running product modulo mod */

    idx_t k = 1;
    for (;;) {
        uint64_t pval = get_prime(n + k);     /* prime(n+k) */
        prod = (prod * (seq_t)pval) % mod;    /* product modulo mod */
        if ((prod + 1) % mod == 0) return (seq_t)k;
        ++k;
    }
}
