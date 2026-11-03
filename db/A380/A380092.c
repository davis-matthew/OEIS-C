/*
 * A380092  Number of consecutive primes after prime(n) before their
 *          concatenation fails to produce a prime.
 * https://oeis.org/A380092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the number of successive primes after prime(n) whose
 *            cumulative concatenation remains a prime
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits_idx, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h>
#include "../utils/num_digits_idx.h"
#include "../utils/sieve_primes.h"
#include "../utils/is_prime_u64.h"

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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Global state for prime generation */
static idx_t *prime_list   = NULL; /* array of primes, 0‑based */
static size_t prime_count  = 0;    /* number of primes stored */
static idx_t  prime_limit  = 0;    /* numeric limit discovered */

/* Ensure we have at least 'needed' primes (1‑based indexing). */
static void ensure_primes_up_to_count(idx_t needed) {
    if (prime_count >= (size_t)needed)
        return;

    if (prime_limit == 0)
        prime_limit = 2;

    while (prime_count < (size_t)needed) {
        prime_limit *= 2;      /* increase numeric upper bound */
        idx_t *tmp = NULL;
        size_t cnt = 0;
        sieve_primes(prime_limit, &tmp, &cnt);
        if (prime_list) free(prime_list);
        prime_list = tmp;
        prime_count = cnt;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, but guard against 0 */
        return 0;

    /* We need at least p_n and p_{n+1} to start. */
    ensure_primes_up_to_count(n + 1);

    idx_t idx    = n - 1;          /* index of prime(n) in 0‑based array */
    seq_t concat = (seq_t)prime_list[idx]; /* current concatenation */
    seq_t count  = 0;              /* answer */

    idx_t next_idx = idx + 1;      /* next prime to try */

    while (true) {
        /* Make sure we have the next prime available. */
        if (next_idx >= prime_count)
            ensure_primes_up_to_count(next_idx + 1);

        /* Concatenate current value with the next prime. */
        int d    = num_digits_idx(prime_list[next_idx]); /* digits of the prime */
        seq_t pow10_d = pow10((idx_t)d);
        seq_t new_concat = concat * pow10_d + (seq_t)prime_list[next_idx];

        /* Test primality of the new concatenation. */
        if (is_prime_big(new_concat)) {
            count++;            /* success */
            concat = new_concat;/* continue */
            next_idx++;
        } else {
            break;              /* failure */
        }
    }

    return count;
}
