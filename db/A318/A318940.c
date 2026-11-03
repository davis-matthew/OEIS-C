/*
 * A318940  Primes whose binary representation contains a consecutive string
 *          of zeros of prime length.
 * https://oeis.org/A318940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 949, u32 n <= 949, u64 n <= 949,
 *            u128 n <= 949, big any n
 * Validated: standard, pass, 66 terms through n = 909 (2026-11-03)
 * Helpers:   bit_length, is_prime_seq, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose binary representation contains a consecutive
 * string of zeros of prime length (search-based implementation). */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/bit_length.h"
#include "../utils/is_prime_seq.h"
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

/* Return true if n, a prime, has a run of consecutive zeros in its binary
 * representation whose length is a prime number. */
static bool has_prime_zero_run(seq_t n)
{
    /* bitlen_seq_t gives the number of bits needed to represent n */
    unsigned long len = bit_length(n);
    unsigned long run = 0;

    for (unsigned long i = 0; i < len; ++i) {
        /* Test the i-th bit (0 = even, 1 = odd).  We shift n right by i
         * and mask with 1; the shift count is an unsigned long, which
         * matches the overload provided by the arbitrary‑precision
         * implementation. */
        if (((n >> i) & 1) == 0) {
            ++run;
        } else {
            if (run > 0 && is_prime_seq((seq_t)run))
                return true;
            run = 0;
        }
    }
    /* If the binary ends with a zero run (not possible for odd primes
     * but kept for completeness). */
    if (run > 0 && is_prime_seq((seq_t)run))
        return true;
    return false;
}

/* Return the n-th term of A318940 (n ≥ 1).  The function works for
 * arbitrary-precision seq_t and idx_t (uint64_t).  If n == 0 the value
 * 0 is returned, but all calls with n ≥ 1 produce the correct prime. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined offset */

    seq_t count    = 0;
    seq_t candidate = 3;                  /* start with the first odd prime > 2 */

    while (1) {
        if (is_prime_big(candidate)) {
            if (has_prime_zero_run(candidate)) {
                ++count;
                if (count == n) return candidate;
            }
        }
        candidate += 2;                   /* test only odd numbers */
    }
}
