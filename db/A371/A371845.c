/*
 * A371845  Primes whose second, third, fourth and fifth digits are 2345.
 * https://oeis.org/A371845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_mod64, pow10, pow_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A371845: primes whose second, third, fourth and fifth decimal digits
   are 2345.  The algorithm enumerates admissible numbers in ascending order
   and returns the nth prime.  Pronholity is tested with a deterministic
   32‑bit Miller‑Rabin routine, which is fast enough for the values required. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/mul_mod64.h"
#include "../utils/pow_mod64.h"

/* ---------- Miller‑Rabin for 32‑bit integers ---------- */

static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;

    /* trial division by small primes */
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }

    /* Miller‑Rabin bases for 32‑bit integers: {2,7,61} */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {2, 7, 61};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t found = 0;

    for (idx_t len = 6; ; ++len) {
        /* 10^(len-1) and 10^(len-5) used for positioning */
        seq_t pow_len_minus1 = pow10(len - 1);
        seq_t pow_len_minus5 = pow10(len - 5);
        seq_t base_sfx = (seq_t)2345 * pow_len_minus5;
        idx_t suffix_limit = (idx_t)pow_len_minus5;  /* number of suffixes 0 … 10^(len-5)-1 */

        for (seq_t d1 = 1; d1 <= 9; ++d1) {
            seq_t prefix = d1 * pow_len_minus1 + base_sfx;
            for (idx_t sfx = 0; sfx < suffix_limit; ++sfx) {
                seq_t candidate = prefix + (seq_t)sfx;
                uint64_t val = (uint64_t)candidate;      /* all candidates < 1e9 for requested n */
                if (is_prime_uint64(val)) {
                    ++found;
                    if (found == n) return candidate;
                }
            }
        }
    }
    return 0;  /* unreachable for valid n */
}
