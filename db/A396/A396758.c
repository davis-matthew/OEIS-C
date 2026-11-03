/*
 * A396758  Larger term of pairs of consecutive primes of the form p, p+6
 *          where both primes eventually reach a value of 1 under iteration of
 *          the sums of the squares of their digits and the first iteration of
 *          both terms are also prime.
 * https://oeis.org/A396758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   get_digits, is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS A396758: larger prime of each consecutive sexy
 * pair of happy primes whose first digit‑square–sum is also prime.
 * The result is the prime p+6 where p and p+6 are consecutive primes,
 * both are happy (their digit‑square‑sum iteration eventually reaches 1),
 * and the first digit‑square‑sum of both is prime. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_digits.h"
#include "../utils/next_prime.h"
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

/* Sum of squares of the decimal digits of a seq_t value. */
static seq_t digit_square_sum_big(seq_t n)
{
    uint8_t digits[128];          /* enough for 128‑bit numbers (≤39 digits) */
    size_t len;
    get_digits(n, digits, &len);
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i)
        sum += (seq_t)(digits[i] * digits[i]); /* 0 … 81 fits in an int */
    return sum;
}

/* Happy number test for a seq_t value. */
static bool is_happy(seq_t n)
{
    if (n < 2) return false;
    seq_t m = n;
    while (true)
    {
        if (m == 1) return true;
        if (m == 4) return false;
        m = digit_square_sum_big(m);
    }
}

/* Checks whether a prime p satisfies the sequence conditions:
 *   - p is happy
 *   - digit_square_sum_big(p) is prime
 */
static bool qualifies_prime(seq_t p)
{
    if (!is_happy(p)) return false;
    seq_t s = digit_square_sum_big(p);
    return is_prime_big(s);
}

/* Memoised storage for the sequence terms. */
static seq_t *terms     = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* Append a new term to the memoised array, reallocating if necessary. */
static void append_term(seq_t val)
{
    if (terms_len == terms_cap)
    {
        size_t new_cap = (terms_cap ? terms_cap * 2 : 16);
        seq_t *new_arr = mem_resize(seq_t, terms, new_cap);
        if (!new_arr) exit(EXIT_FAILURE); /* out‑of‑memory */
        terms = new_arr;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* Main sequence function required by the interface. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety for indices below the offset */

    static bool initialized = false;
    static uint64_t prev_prime = 2;   /* prime to start from */

    if (!initialized)
    {
        initialized = true;
    }

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n)
    {
        /* Obtain the next consecutive prime pair (p, q). */
        uint64_t p = next_prime(prev_prime);   /* lower prime of the pair */
        uint64_t q = next_prime(p);            /* upper prime of the pair */

        if (q == p + 6)   /* sexy prime pair */
        {
            seq_t sp = (seq_t)p;
            seq_t sq = (seq_t)q;

            if (qualifies_prime(sp) && qualifies_prime(sq))
                append_term(sq);   /* larger term becomes a sequence entry */
        }

        prev_prime = p;   /* advance to the next prime in the list */
    }

    return terms[n - 1];
}
