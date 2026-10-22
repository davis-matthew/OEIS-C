/*
 * A321701  Primes that are not base-5 deletable primes (written in base 10).
 * https://oeis.org/A321701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 6000, u32 n <= 9848, u64 n <= 9848,
 *            u128 n <= 9848, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A321701 – primes that are not base‑5 deletable primes
   (n ≥ 1).  The function iterates over primes in increasing order
   and returns the nth prime that fails the recursive deletability
   test.  It works for all supported seq_t / idx_t types. */

#include <stdbool.h>
#include <stdint.h>

/* is_prime is provided by ../utils/is_prime.h for idx_t (uint64_t). */
#include "../utils/is_prime.h"

/* ---------- helper: base‑5 deletability test for uint64_t numbers ---------- */
static bool deletable_uint64_recursive(uint64_t n)
{
    /* Convert n to base‑5 digits, most significant first. */
    uint8_t dig[64];
    int len = 0;
    uint64_t t = n;
    if (t == 0) {
        dig[0] = 0;
        len = 1;
    } else {
        while (t > 0 && len < (int)sizeof(dig)) {
            dig[len++] = t % 5;
            t /= 5;
        }
        /* Reverse to most significant first */
        for (int i = 0; i < len/2; ++i) {
            uint8_t tmp = dig[i];
            dig[i] = dig[len-1-i];
            dig[len-1-i] = tmp;
        }
    }

    /* A single digit is always deletable (remove it → empty string). */
    if (len == 1) return true;

    /* Try deleting each digit that does not create a leading zero. */
    for (int k = 0; k < len; ++k) {
        if (k == 0 && dig[1] == 0) continue; /* would have leading zero */
        uint64_t new_val = 0;
        for (int j = 0; j < len; ++j) {
            if (j == k) continue;
            new_val = new_val * 5 + dig[j];
        }
        if (!is_prime(new_val)) continue;       /* must be prime */
        if (deletable_uint64_recursive(new_val)) return true;
    }
    return false;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined below offset 1 */

    uint64_t found = 0;
    uint64_t cand = 2;

    for (;;) {
        if (!is_prime(cand)) {                /* skip composites */
            ++cand;
            continue;
        }
        if (!deletable_uint64_recursive(cand)) {
            ++found;
            if (found == n) return (seq_t)cand;
        }
        ++cand;
    }
}
