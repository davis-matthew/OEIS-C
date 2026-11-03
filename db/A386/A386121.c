/*
 * A386121  Primes having only {1, 4, 5, 8} as digits.
 * https://oeis.org/A386121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   enumerates the primes whose decimal digits are only in {1,4,5,8}
 * Range:     u8 n <= 5, u16 n <= 72, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 9881 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A386121 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

static const uint8_t digit_map[4] = {1, 4, 5, 8};

/* Generate the Nth allowed-digit number in ascending order.
   len  : number of digits
   pos  : index in 0 .. 4^len-1
   Returns the number as seq_t. */
static seq_t
generate_candidate(idx_t len, idx_t pos)
{
    seq_t val = 0;
    uint8_t digits[20];              /* len is always < 20 for 64‑bit range */
    idx_t tmp = pos;
    /* Build digits from most significant to least significant */
    for (idx_t i = len; i > 0; --i) {
        uint8_t d = tmp % 4;
        tmp /= 4;
        digits[i - 1] = digit_map[d];
    }
    for (idx_t i = 0; i < len; ++i) {
        val = val * 10 + digits[i];
    }
    return val;
}

/* Dynamic array of primes found so far */
static seq_t  *prime_list    = NULL;
static idx_t   prime_len    = 0;
static idx_t   prime_cap    = 0;

/* Iterator state over allowed-digit numbers */
static idx_t   cur_len      = 1;   /* current length of numbers being enumerated */
static idx_t   cur_pos      = 0;   /* current position within the current length */
static idx_t   cur_limit    = 4;   /* 4^cur_len */

/* Ensure that prime_list contains at least n primes.
   On exit prime_len >= n. */
static void
ensure_primes(idx_t n)
{
    while (prime_len < n) {
        seq_t candidate = generate_candidate(cur_len, cur_pos);

        /* Advance iterator */
        ++cur_pos;
        if (cur_pos == cur_limit) {
            ++cur_len;
            cur_limit *= 4;
            cur_pos = 0;
        }

        /* Accept only primes that end with 1 (except the single digit 5) */
        if (!(candidate == 5 || (candidate % 10) == 1))
            continue;

        if (isPrimeGeneric(candidate)) {
            if (prime_len == prime_cap) {
                idx_t new_cap = prime_cap ? prime_cap * 2 : 32;
                seq_t *tmp = mem_resize(seq_t, prime_list, new_cap);
                if (!tmp) exit(EXIT_FAILURE);
                prime_list = tmp;
                prime_cap = new_cap;
            }
            prime_list[prime_len++] = candidate;
        }
    }
}

/* Main sequence function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, n<1 is undefined */
    ensure_primes(n);
    return prime_list[n - 1];
}
