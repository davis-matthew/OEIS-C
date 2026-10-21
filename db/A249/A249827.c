/*
 * A249827  Row 3 of A246278: replace in 2n each prime factor p(k) with prime
 *          p(k+2).
 * https://oeis.org/A249827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π prime_{k+2}^{e_k} over all prime factors prime_k of 2*n
 *            where e_k is the exponent of prime_k in 2*n (counted
 *            multiplicity). The sequence is row 3 of A246278 – replace each
 *            prime factor p_k of 2n with the prime two positions ahead in the
 *            ordered list of primes
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 8191, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Prime sieve and lookup ---------- */

static uint64_t  *prime_table = NULL;   /* stored primes, 0‑based */
static size_t    prime_count = 0;       /* number of primes stored   */
static uint64_t  prime_limit = 0;      /* largest prime value cached */

/* Build all primes up to 'limit' (inclusive).  The table is rebuilt
   from scratch whenever the limit grows.  The algorithm uses a simple
   sieve of Eratosthenes and works on uint64_t, which is
   sufficient for the required ranges. */
static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= prime_limit) return;           /* already sufficient */

    /* allocate sieve array (true = composite) */
    uint8_t *sieve = (uint8_t*)calloc(limit + 1, 1);
    if (!sieve) { _Exit(1); }

    /* simple sieve */
    uint64_t *tmp = NULL;
    size_t tmp_sz = 0;
    size_t tmp_cap = 0;

    for (uint64_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {                      /* prime found */
            if (tmp_sz >= tmp_cap) {
                tmp_cap = tmp_cap ? tmp_cap * 2 : 16;
                uint64_t *newptr = (uint64_t*)realloc(tmp, tmp_cap * sizeof(uint64_t));
                if (!newptr) { free(sieve); _Exit(1); }
                tmp = newptr;
            }
            tmp[tmp_sz++] = i;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    sieve[j] = 1;
            }
        }
    }

    free(sieve);
    free(prime_table);
    prime_table = tmp;
    prime_count = tmp_sz;
    prime_limit = limit;
}

/* Binary search for index of prime p in prime_table (1‑based).  It
   assumes that p is indeed prime and present in the table. */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (prime_table[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return lo + 1;            /* 1‑based */
}

/* Return the prime with 1‑based index 'idx'.
   If the table does not contain enough primes, it is extended
   by doubling the sieve upper bound until the required index is
   available. */
static seq_t prime_by_index(size_t idx)
{
    if (idx <= 0) return 0;
    if (idx > prime_count) {
        uint64_t new_limit = prime_limit ? prime_limit * 2 : 11;
        while (prime_count < idx) {
            ensure_primes_up_to(new_limit);
            new_limit *= 2;          /* double again for next round */
        }
    }
    return (seq_t)prime_table[idx - 1];
}

/* ---------- Sequence computation ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but safe */

    /* m = 2*n : the integer to factorize */
    seq_t m = (seq_t)n << 1;       /* 2*n */

    /* Ensure we have at least the primes needed for factorization
       up to sqrt(m).  Using m itself as an upper bound is safe. */
    ensure_primes_up_to((uint64_t)m);

    seq_t result = 1;
    seq_t temp   = m;

    /* Trial division by stored primes */
    for (size_t i = 0; i < prime_count; ++i) {
        seq_t p = (seq_t)prime_table[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            /* exponent of this prime */
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }

            size_t idx_p = prime_index((uint64_t)p);   /* 1‑based index of p */
            size_t rep_idx = idx_p + 2;               /* replace by prime_{idx_p+2} */
            seq_t rep = prime_by_index(rep_idx);

            while (exp-- > 0) result *= rep;
        }
    }

    /* Remaining factor (if any) is prime */
    if (temp > 1) {
        uint64_t rem = (uint64_t)temp;    /* safe: rem <= m */
        size_t idx_rem = prime_index(rem);
        size_t rep_idx = idx_rem + 2;
        seq_t rep = prime_by_index(rep_idx);
        result *= rep;
    }

    return result;
}
