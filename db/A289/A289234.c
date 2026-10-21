/*
 * A289234  In primorial base: a(n) is obtained by replacing each nonzero
 *          digit of n with its inverse (see Comments for precise definition).
 * https://oeis.org/A289234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 251, u16 n <= 9548, u32 n <= 9548, u64 n <= 9548,
 *            u128 n <= 9548, big any n
 * Helpers:   _mod_inverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Compute the primorial‑base inverse permutation
   A289234.  For n:
   1. Repeatedly divide n by successive primes p_1=2, p_2=3, … .
      Let d_i = n % p_{i+1} be the i‑th digit (0 ≤ d_i < p_{i+1}).
   2. If d_i > 0 replace it by its modular inverse e_i
      (e_i*d_i ≡ 1 (mod p_{i+1})), otherwise e_i = 0.
   3. Rebuild the number using the primorial bases
      B_i = ∏_{j=0}^{i-1} p_{j+1} (B_0 = 1).  Return the result. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/_mod_inverse.h"

/* ---------- Prime generation ------------------------------------------ */

static uint64_t primes[1024] = {0};  /* will store the first primes */
static size_t prime_count = 0;      /* number of primes known so far */

/* Ensure that the table contains at least the first k primes (k ≥ 1). */
static void _ensure_primes(size_t k)
{
    if (prime_count >= k)
        return;

    if (prime_count == 0) {
        primes[prime_count++] = 2;
    }

    while (prime_count < k) {
        uint64_t cand = primes[prime_count - 1] + 1;   /* start at next integer */
        for (;;) {
            bool ok = true;
            for (size_t i = 0; i < prime_count && primes[i] * primes[i] <= cand; ++i) {
                if (cand % primes[i] == 0) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                primes[prime_count++] = cand;
                break;
            }
            ++cand;
        }
    }
}

/* Return the (k‑th, 1‑based) prime.  k ≥ 1. */
static uint64_t _get_prime(size_t k)
{
    _ensure_primes(k);
    return primes[k - 1];
}

/* ---------- Modular inverse ------------------------------------------- */

/* ---------- Sequence implementation ----------------------------------- */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t mult   = 1;   /* current primorial base B_i */
    idx_t rem    = n;
    idx_t i      = 0;   /* digit index */

    while (rem > 0) {
        uint64_t prime = _get_prime((size_t)(i + 1));       /* p_{i+1} */
        seq_t digit    = rem % prime;                      /* d_i */
        rem           = rem / prime;                       /* next reduced n */

        seq_t inv = 0;
        if (digit != 0) {
            inv = (seq_t)_mod_inverse((uint64_t)digit, prime);
        }

        result += inv * mult;          /* add e_i * B_i */
        mult   *= (seq_t)prime;        /* prepare B_{i+1} */
        ++i;
    }

    return result;
}
