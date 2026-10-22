/*
 * A373838  a(n) = 1 if n and A276150(n) are both multiples of 3, otherwise 0,
 *          where A276150 is the digit sum in the primorial base.
 * https://oeis.org/A373838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if n and A276150(n) are both multiples of 3, otherwise
 *            0
 * Range:     u8 n <= 2852, u16 n <= 9947, u32 n <= 9947, u64 n <= 9947,
 *            u128 n <= 9947, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------- */
/* Prime list maintained on demand                                   */
static uint32_t *prime_list = NULL;   /* array of primes  */
static size_t   prime_cnt   = 0;       /* number of primes stored */

/* Ensure that prime_list has at least (idx+1) primes available.   */
static void ensure_prime_count(size_t idx)
{
    if (prime_cnt > idx) return;

    /* Increase capacity enough for idx. */
    size_t new_cap = prime_cnt == 0 ? 16 : prime_cnt * 2;
    while (new_cap <= idx) new_cap <<= 1;

    uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
    if (!tmp) exit(1);                 /* abort on allocation failure */
    prime_list = tmp;

    /* Generate missing primes starting after the last one we have. */
    uint64_t candidate = (prime_cnt == 0) ? 2 : (uint64_t)prime_list[prime_cnt - 1] + 1;
    while (prime_cnt < new_cap) {
        bool is_p = true;
        for (size_t i = 0; i < prime_cnt &&
             (uint64_t)prime_list[i] * prime_list[i] <= candidate; ++i) {
            if (candidate % prime_list[i] == 0) { is_p = false; break; }
        }
        if (is_p) {
            prime_list[prime_cnt++] = (uint32_t)candidate;
        }
        ++candidate;
    }
}

/* Return the (0‑based) prime at position idx. */
static uint32_t primorial_prime(size_t idx)
{
    ensure_prime_count(idx);
    return prime_list[idx];
}

/* --------------------------------------------------------------- */
/* Compute A276150(n): digit sum of n in primorial base.           */
static seq_t primorial_digit_sum(idx_t n)
{
    seq_t sum = (seq_t)0;
    idx_t rem = n;
    size_t k = 0;
    while (rem > 0) {
        uint32_t p = primorial_prime(k);
        seq_t digit = (seq_t)(rem % p);
        sum += digit;
        rem /= (idx_t)p;
        ++k;
    }
    return sum;
}

/* --------------------------------------------------------------- */
/* Public sequence function                                       */
seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t dsum = primorial_digit_sum(n);

    /* Test divisibility by 3 for both n and the digit sum.       */
    seq_t out = ((nseq % (seq_t)3) == (seq_t)0 &&
                 (dsum % (seq_t)3) == (seq_t)0) ? (seq_t)1 : (seq_t)0;
    return out;
}
