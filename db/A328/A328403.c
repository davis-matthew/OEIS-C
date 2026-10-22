/*
 * A328403  a(n) = A276086(A276086(A276086(n))), where A276086(n) converts
 *          primorial base expansion of n into its prime product form.
 * https://oeis.org/A328403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A276086(A276086(A276086(n)))
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 11, u64 n <= 17, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primorial‐base transformation:  n → ∏ p_i^d_i   */
/* where d_i = n mod p_i and n = n / p_i   */
/* The sequence is evaluated for any n ≥ 0          */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"

/* Prime cache (always fetched as uint64_t).  The cache is
   shared between calls and is lazily initialised. */
static uint64_t *prime_cache = NULL;
static size_t     prime_cnt   = 0;
static bool       primes_init = false;

/* Initialise prime cache once */
static void init_primes(void)
{
    if (primes_init) return;
    /* A small upper bound that covers primorial bases for n ≤ 10 000
       and keeps the allocation modest.  */
    idx_t limit = 2000000;
    build_primes_upto(limit, &prime_cache, &prime_cnt);
    primes_init = true;
}

/* Apply the A276086 transformation once.
   Input and output are of type seq_t to support arbitrary precision. */
static seq_t primorial_transform(seq_t n)
{
    init_primes();

    seq_t result = (seq_t)1;
    size_t i = 0;
    while (n > (seq_t)0 && i < prime_cnt) {
        seq_t p     = (seq_t)prime_cache[i];
        seq_t digit = n % p;          /* digit in primorial base */
        if (digit != (seq_t)0) {
            result *= big_pow(p, digit);
        }
        n /= p;                       /* move to next prime radix */
        ++i;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    x = primorial_transform(x);
    x = primorial_transform(x);
    x = primorial_transform(x);
    return x;
}
