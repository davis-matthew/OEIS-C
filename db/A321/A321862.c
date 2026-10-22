/*
 * A321862  a(n) = A321857(prime(n)).
 * https://oeis.org/A321862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -∑_{i=1..n} Legendre(prime(i),5)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Legendre(p,5) = 1  if p≡1 or 4 (mod 5)            */
/*                = –1 if p≡2 or 3 (mod 5)            */
/*                =  0  if p = 5                     */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Computing a(n) for A321862
   (sequence defined by the sum of Legendre symbols of the first n primes) */
seq_t a(idx_t n)
{
    /* ---------- static cache of previously computed values ---------- */
    static seq_t *arr  = NULL;   /* arr[0] = a(1), arr[1] = a(2), ... */
    static size_t arr_cap = 0;
    static idx_t  computed = 0;  /* number of values stored in arr */

    /* ---------- static cache of generated primes ---------- */
    static uint64_t *primes   = NULL;  /* primes[0] = 2, primes[1] = 3, ... */
    static size_t    prime_cap = 0;
    static idx_t     prime_cnt = 0;    /* number of primes stored */

    /* ---------- guard against calls with offset 0 ---------- */
    if (n == 0) return 0;

    /* ---------- initialise on first call ---------- */
    if (computed == 0) {
        /* a array */
        arr_cap = 16;
        arr = (seq_t *)calloc(arr_cap, sizeof(seq_t));

        /* primes array */
        prime_cap = 16;
        primes = (uint64_t *)calloc(prime_cap, sizeof(uint64_t));
        primes[0] = 2;
        prime_cnt = 1;

        /* a(1) */
        arr[0] = 1;          /* -Legendre(2,5) = –( –1 ) = 1 */
        computed = 1;
    }

    /* ---------- enlarge a array if required ---------- */
    if (n > arr_cap) {
        size_t new_cap = arr_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure – abort */
        arr = tmp;
        arr_cap = new_cap;
    }

    /* ---------- generate primes until prime_cnt >= n ---------- */
    while (prime_cnt < n) {
        uint64_t cand = primes[prime_cnt-1] + 1;
        /* simple primality test using the primes already found */
        while (true) {
            bool ok = true;
            for (idx_t j = 0; j < prime_cnt; ++j) {
                uint64_t p = primes[j];
                if ((unsigned __int128)p * p > cand) break;
                if (cand % p == 0) { ok = false; break; }
            }
            if (ok) break;
            ++cand;
        }

        /* grow primes array if needed */
        if ((size_t)prime_cnt >= prime_cap) {
            size_t new_cap = prime_cap;
            while ((size_t)prime_cnt >= new_cap) new_cap <<= 1;
            uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
            if (!tmp) abort();
            primes = tmp;
            prime_cap = new_cap;
        }

        primes[prime_cnt++] = cand;   /* store the new prime */
    }

    /* ---------- compute a values up to n ---------- */
    seq_t cur = arr[computed-1];     /* value of a(computed) */
    for (idx_t k = computed+1; k <= n; ++k) {
        uint64_t p = primes[k-1];    /* prime(k) */
        int leg;
        if (p == 5) {
            leg = 0;
        } else {
            uint64_t r = p % 5;
            leg = (r == 1 || r == 4) ? 1 : -1;
        }
        cur += -(seq_t)leg;         /* a(k) = a(k-1) - Legendre(p,5) */
        arr[k-1] = cur;
    }
    computed = n;

    return arr[n-1];
}
