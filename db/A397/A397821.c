/*
 * A397821  Squarefree k whose third smallest distinct prime factor is smaller
 *          than the square of the least prime factor.
 * https://oeis.org/A397821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th odd, square‑free integer with at least three
 *            distinct prime factors, whose third smallest distinct prime
 *            factor is less than the square of the least prime factor
 * Range:     u8 n <= 1, u16 n <= 1243, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* helper: integer square root (floor) for 64‑bit numbers */
static idx_t floor_sqrt_u64(idx_t n) {
    idx_t lo = 0, hi = (n < (idx_t)0xFFFFFFFFu ? n : (idx_t)0xFFFFFFFFu), res = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) { lo = 1; continue; }
        if (mid > n / mid) {          /* mid*mid would overflow or > n */
            hi = mid - 1;
        } else {
            idx_t sq = mid * mid;     /* safe: mid*mid <= n <= 2^64-1 */
            if (sq == n) return mid;
            if (sq < n) {
                res = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
    }
    return res;
}

/* dynamic prime list */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0, prime_cap = 0;

/* extend prime list up to at least 'limit' */
static void ensure_primes_up_to(idx_t limit) {
    if (prime_count && prime_list[prime_count - 1] >= limit) return;
    idx_t start = (prime_count == 0) ? 2 : prime_list[prime_count - 1] + 1;
    for (idx_t p = start; p <= limit; ++p) {
        bool is_prime = true;
        idx_t r = floor_sqrt_u64(p);
        for (size_t j = 0; j < prime_count && prime_list[j] <= r; ++j) {
            if (p % prime_list[j] == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_count == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_count++] = p;
        }
    }
}

/* test whether an odd number k satisfies the sequence condition */
static bool satisfies(uint64_t k) {
    uint64_t temp = k;

    /* gather distinct primes, check square‑free */
    uint64_t first_p = 0, second_p = 0, third_p = 0;
    size_t prime_ct = 0;
    idx_t limit = floor_sqrt_u64(k);
    ensure_primes_up_to(limit);

    for (size_t i = 0; i < prime_count && prime_list[i] * (uint64_t)prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            size_t cnt = 0;
            while (temp % p == 0) { temp /= p; ++cnt; }
            if (cnt > 1) return false;      /* not square‑free */

            ++prime_ct;
            if (first_p == 0) first_p = p;
            else if (second_p == 0) second_p = p;
            else if (third_p == 0) third_p = p;
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        ++prime_ct;
        if (first_p == 0) first_p = temp;
        else if (second_p == 0) second_p = temp;
        else if (third_p == 0) third_p = temp;
    }

    if (prime_ct < 3) return false;

    /* third prime must be < (first prime)^2 */
    if (third_p / first_p >= first_p) return false;

    return true;
}

/* static storage for the sequence values */
static seq_t *terms = NULL;
static idx_t terms_size = 0, terms_cap = 0;
static uint64_t next_candidate = 3;     /* start at the first odd >1 */

/* public function */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset is 1; n==0 is unused */

    /* generate terms until we have at least n */
    while (terms_size < n) {
        while (true) {
            uint64_t k = next_candidate;
            next_candidate += 2;            /* next odd number */

            if (satisfies(k)) {
                if (terms_size == terms_cap) {
                    terms_cap = terms_cap ? terms_cap * 2 : 16;
                    terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                }
                terms[terms_size++] = (seq_t)k;
                break;
            }
        }
    }
    return terms[n - 1];
}
