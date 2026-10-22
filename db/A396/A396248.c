/*
 * A396248  Primes prime(k) such that prime(k) - prime(k-1) is a factorial.
 * https://oeis.org/A396248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime_small, realloc_void
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A396248: primes whose gap to the previous prime is a factorial */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/realloc_void.h"

/* Small factorials that can occur as prime gaps. */
static const uint64_t fact_list[] = {
    1ULL,   /* 0! */
    2ULL,   /* 1! = 2    */
    6ULL,   /* 2! = 6    */
    24ULL,  /* 3! = 24   */
    120ULL, /* 4! =120   */
    720ULL, /* 5! =720   */
    5040ULL,/* 6! =5040  */
    40320ULL,/* 7! =40320 */
    362880ULL,/* 8! =362880 */
    3628800ULL,/* 9! =3628800 */
    39916800ULL,/*10! =39916800*/
    479001600ULL,/*11! =479001600*/
    6227020800ULL,/*12! =6227020800*/
    87178291200ULL,/*13! =87178291200*/
    1307674368000ULL,/*14! =1307674368000*/
    20922789888000ULL,/*15! =20922789888000*/
    355687428096000ULL,/*16! =355687428096000*/
    6402373705728000ULL,/*17! =6402373705728000*/
    121645100408832000ULL,/*18! =121645100408832000*/
    2432902008176640000ULL /*19! =2432902008176640000 */
};
static const size_t fact_cnt = sizeof(fact_list) / sizeof(fact_list[0]);

static inline bool is_factorial(uint64_t x) {
    for (size_t i = 0; i < fact_cnt; ++i) {
        if (fact_list[i] == x) return true;
        if (fact_list[i] > x) break;
    }
    return false;
}

/* Dynamically growing arrays that store prime numbers (uint64_t)
   and the computed sequence elements (seq_t). */
static uint64_t *prime_arr = NULL;
static seq_t    *seq_arr    = NULL;
static size_t    prime_cnt  = 0, seq_cnt  = 0;
static size_t    prime_cap  = 0, seq_cap   = 0;

static inline void ensure_prime_cap(size_t need) {
    if (prime_cap < need) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 128;
        while (new_cap < need) new_cap <<= 1;
        prime_arr = (uint64_t*)realloc_void(prime_arr, new_cap * sizeof(uint64_t));
        prime_cap = new_cap;
    }
}

static inline void ensure_seq_cap(size_t need) {
    if (seq_cap < need) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 128;
        while (new_cap < need) new_cap <<= 1;
        seq_arr = (seq_t*)realloc_void(seq_arr, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* safety for non‑positive indexes */

    /* Generate terms until we have at least n of them. */
    while (seq_cnt < (size_t)n) {
        /* First call: initialise the prime list with the first prime. */
        if (prime_cnt == 0) {
            ensure_prime_cap(1);
            prime_arr[prime_cnt++] = 2ULL; /* start with 2 */
            /* no sequence element yet */
        }

        uint64_t prev = prime_arr[prime_cnt - 1];
        uint64_t cand = (prev == 2ULL) ? 3ULL : prev + 2ULL; /* only odd candidates */

        while (!is_prime_small(cand)) cand += 2ULL; /* find next prime */

        ensure_prime_cap(prime_cnt + 1);
        prime_arr[prime_cnt++] = cand;

        uint64_t diff = cand - prev;
        if (is_factorial(diff)) {
            ensure_seq_cap(seq_cnt + 1);
            seq_arr[seq_cnt++] = (seq_t)cand;
        }
        /* continue the loop to generate more primes as needed */
    }

    return seq_arr[n - 1];
}
