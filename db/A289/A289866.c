/*
 * A289866  Primes obtained from other primes by prefixing a 1.
 * https://oeis.org/A289866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth prime obtained by prefixing the digit 1 to a
 *            prime p and the result is also prime
 * Range:     u8 n <= 9, u16 n <= 231, u32 n <= 2170, u64 n <= 2170,
 *            u128 n <= 2170, big any n
 * Validated: standard, pass, 60 terms through n = 1882 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Persistent state for memoization */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;       /* number of terms stored */
    static size_t seq_cap = 0;       /* current capacity of seq */

    static uint64_t *primes = NULL;  /* array of primes */
    static size_t prime_cnt = 0;     /* number of primes in the array */
    static size_t prime_index = 0;   /* next prime to process */
    static uint64_t prime_limit = 0; /* highest limit used for primes */

    /* One‑time initialization */
    if (prime_limit == 0) {
        prime_limit = 1000000ULL;                /* start with 1 000 000 */
        build_primes_upto(prime_limit, &primes, &prime_cnt);
        seq_cap = 32;
        seq = mem_new(seq_t, seq_cap);
        seq_len = 0;
    }

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        if (prime_index >= prime_cnt) {          /* need more primes */
            uint64_t new_limit = prime_limit * 2ULL;
            free(primes);
            build_primes_upto(new_limit, &primes, &prime_cnt);
            prime_limit = new_limit;
        }

        uint64_t p = primes[prime_index++];     /* next prime p */

        /* count digits of p */
        uint64_t tmp = p;
        uint64_t d = 0;
        do { ++d; tmp /= 10; } while (tmp);

        /* construct q = 10^d + p  (prefixing a 1) */
        seq_t q = pow10((idx_t)d) + (seq_t)p;

        /* test whether q is prime */
        if (is_prime_big(q)) {
            if (seq_len >= seq_cap) {           /* grow storage */
                seq_cap = seq_cap * 2;
                seq = mem_resize(seq_t, seq, seq_cap);
            }
            seq[seq_len++] = q;
        }
    }

    return seq[(size_t)n - 1];
}
