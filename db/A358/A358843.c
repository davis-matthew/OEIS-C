/*
 * A358843  Numbers k such that A276086(k) == 5 (mod 6), where A276086 is the
 *          primorial base exp-function.
 * https://oeis.org/A358843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that the primorial base
 *            exp‑function A276086(k) ≡ 5 (mod 6)
 * Range:     u8 n <= 19, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation treats k as the sequence value directly. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* maximum number of digits that will ever be required for the queried n ≤ 10000 */
#define MAX_DIGITS 200

/* static storage for primes needed for the mixed radix base */
static uint32_t primes[MAX_DIGITS];
static size_t prime_cnt = 0;
static bool primes_init = false;

/* generate the first MAX_DIGITS primes on first use */
static void init_primes(void)
{
    if (primes_init) return;
    prime_cnt = 0;
    primes[prime_cnt++] = 2;
    uint32_t candidate = 3;
    while (prime_cnt < MAX_DIGITS) {
        bool is_prime = true;
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            if ((uint64_t)p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) primes[prime_cnt++] = candidate;
        candidate += 2;
    }
    primes_init = true;
}

/* dynamic array for the sequence values */
static seq_t *seq = NULL;
static size_t seq_len = 0, seq_cap = 0;

/* mixed radix digits and current value of k */
static uint32_t digits[MAX_DIGITS];
static size_t digit_len = 2;      /* need at least two digits for the condition */
static idx_t k_counter = 0;       /* number represented by the current digits */

/* increment the mixed radix representation */
static void inc_digits(void)
{
    size_t i = 0;
    while (1) {
        ++digits[i];
        if (digits[i] == primes[i]) {          /* carry to next digit */
            digits[i] = 0;
            ++i;
            if (i >= digit_len) {
                digits[i] = 0;
                ++digit_len;
            }
        } else {
            break;
        }
    }
}

/* ensure capacity of the sequence array */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!new_seq) exit(EXIT_FAILURE);  /* allocation failed */
    seq = new_seq;
    seq_cap = new_cap;
}

/* main function requested by the exercise */
seq_t a(idx_t n)
{
    init_primes();

    if (seq_len < (size_t)n) {
        /* generate sequence until we have at least n elements */
        while (seq_len < (size_t)n) {
            inc_digits();
            ++k_counter;

            /* compute A276086(k) modulo 6 and count odd exponents for primes ≡5 (mod 6) */
            int prod_mod6 = 1;
            int odd_5mod6 = 0;

            for (size_t i = 0; i < digit_len; ++i) {
                uint32_t exp = digits[i];
                if (exp == 0) continue;
                uint32_t p = primes[i];
                int val = 1;
                for (uint32_t j = 0; j < exp; ++j)
                    val = (val * p) % 6;
                prod_mod6 = (prod_mod6 * val) % 6;
                if (p % 6 == 5 && (exp & 1)) ++odd_5mod6;
            }

            /* condition: no factor 2 or 3, odd number of (5 mod 6) primes with odd exponent */
            if (digits[0] == 0 && digits[1] == 0 && (odd_5mod6 & 1)) {
                ensure_seq_cap(seq_len + 1);
                seq[seq_len++] = (seq_t)k_counter;
            }
        }
    }
    return seq[(size_t)n - 1];
}
