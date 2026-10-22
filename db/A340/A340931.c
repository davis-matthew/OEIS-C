/*
 * A340931  Heinz numbers of integer partitions of odd numbers into an odd
 *          number of parts.
 * https://oeis.org/A340931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Heinz number of integer partitions of odd numbers into an
 *            odd number of parts
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Dynamic storage of primes (1‑based indices) */
static uint64_t *prime_list = NULL;
static size_t   prime_cnt   = 0;
static size_t   prime_cap   = 0;
static uint64_t prime_candidate = 3;  /* next odd number to test for primality */

/* Ensure prime_list contains all primes ≤ limit */
static void ensure_primes_up_to(uint64_t limit)
{
    if (prime_cnt == 0) {          /* first time: add prime 2 */
        prime_cap   = 16;
        prime_list  = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_list[0] = 2;
        prime_cnt = 1;
        prime_candidate = 3;
    }
    while (prime_list[prime_cnt - 1] < limit) {
        uint64_t cand = prime_candidate;
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t q = prime_list[i];
            if (q > cand / q) break;          /* avoid overflow */
            if (cand % q == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = cand;
        }
        prime_candidate += 2;   /* only odd numbers */
    }
}

/* Return 1‑based index of prime p (p is guaranteed to be prime) */
static idx_t prime_index_uint64(uint64_t p)
{
    ensure_primes_up_to(p);           /* ensures all primes ≤ p are present */
    /* binary search */
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    /* lo is the index of p */
    return (idx_t)(lo + 1);           /* 1‑based */
}

/* Dynamic array for the sequence values */
static seq_t *seq_vals  = NULL;
static size_t seq_used  = 0;
static size_t seq_cap   = 0;
static uint64_t next_candidate = 2;   /* next integer to test */

/* Check whether integer m satisfies: odd number of parts and odd sum of indices */
static bool valid_candidate(uint64_t m)
{
    uint64_t tmp = m;
    int  parts      = 0;
    int  sum_index  = 0;
    size_t i = 0;
    /* Ensure we have enough primes to factor up to sqrt(m) */
    ensure_primes_up_to(m);
    for (i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            parts      += e;
            sum_index += e * (int)(i + 1);   /* primes are 0‑based; index = i+1 */
        }
    }
    if (tmp > 1) {   /* remaining prime factor */
        idx_t idx = prime_index_uint64(tmp);
        parts      += 1;
        sum_index += (int)idx;
    }
    return ((parts & 1) != 0) && ((sum_index & 1) != 0);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n=0; return 0 */

    while (seq_used < (size_t)n) {
        while (!valid_candidate(next_candidate))
            ++next_candidate;
        /* store the candidate */
        if (seq_used == seq_cap) {
            seq_cap = seq_cap ? seq_cap * 2 : 16;
            seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
        }
        seq_vals[seq_used++] = (seq_t)next_candidate;
        ++next_candidate;
    }
    return seq_vals[(size_t)n - 1];
}
