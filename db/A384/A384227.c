/*
 * A384227  Smaller members p of sexy prime pairs (p,p+6) such that both p and
 *          p+6 are emirps.
 * https://oeis.org/A384227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th prime p such that p and p+6 are prime, both are
 *            emirps (different from their decimal reversal which is also
 *            prime)
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   is_prime_u64, mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/reverse_digits.h"
#include "../utils/mem.h"
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

/* Memoized list of sequence values. */
static seq_t *results = NULL;
static size_t res_size = 0;      /* number of stored terms in `results`. */
static idx_t current_limit = 0;  /* upper bound of the sieve in the last rebuild. */

/* Rebuild the results list up to at least `need` terms by extending
   the sieve to a larger limit. */
static void extend_and_recompute(idx_t need)
{
    if (current_limit == 0) current_limit = 100000;   /* start with a sane limit */

    /* Increase the limit until we have enough terms. */
    while (res_size < (size_t)need) {
        current_limit *= 2;   /* exponential growth */

        /* Allocate and run a classic Eratosthenes sieve up to `current_limit`. */
        uint8_t *isPrime = (uint8_t *)malloc((size_t)current_limit + 1);
        if (!isPrime) abort();
        memset(isPrime, 1, (size_t)current_limit + 1);
        isPrime[0] = isPrime[1] = 0;

        for (idx_t i = 2; i * i <= current_limit; ++i) {
            if (isPrime[i]) {
                for (idx_t j = i * i; j <= current_limit; j += i)
                    isPrime[j] = 0;
            }
        }

        /* Rebuild the results list from scratch. */
        mem_free(results);        /* discard old list */
        results = NULL;
        res_size = 0;

        /* Rough capacity guess; will grow if needed. */
        size_t cap = (size_t)(current_limit / 20 + 1);
        results = mem_new(seq_t, cap);
        if (!results) abort();

        for (idx_t p = 2; p + 6 <= current_limit; ++p) {
            if (!isPrime[p] || !isPrime[p + 6]) continue;

            seq_t pp = (seq_t)p;
            seq_t qp = pp + 6;

            seq_t rp = reverse_digits(pp);
            seq_t rq = reverse_digits(qp);

            /* Skip if either number is non‑emirp:
               - it's a palindrome (reverse equals itself),
               - or its reverse is not prime. */
            if (rp == pp || !is_prime_big(rp)) continue;
            if (rq == qp || !is_prime_big(rq)) continue;

            /* Append pp to results. */
            if (res_size == cap) {
                cap *= 2;
                seq_t *tmp = mem_resize(seq_t, results, cap);
                if (!tmp) abort();
                results = tmp;
            }
            results[res_size++] = pp;
        }
        free(isPrime);
    }
}

/* Sequence function requested by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; behaviour for 0 is irrelevant */

    if (res_size < (size_t)n) extend_and_recompute(n);
    return results[n - 1];    /* convert 1‑based index to 0‑based array index */
}
