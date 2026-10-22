/*
 * A398551  Even numbers that are not sums of two isolated primes.
 * https://oeis.org/A398551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A398551: Even numbers that are not sums of two isolated primes.
   The function builds the list lazily on first call by:
   - Sieve primes up to a fixed limit LIMIT.
   - Identify isolated primes (primes p with neither p‑2 nor p+2 prime).
   - Mark all even sums of two isolated primes.
   - Store all remaining even numbers up to LIMIT in a static array.
   Any index n ≥ 1 is answered as the (n‑1)‑th element of that array,
   returning 0 if n exceeds the number of terms stored. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define LIMIT 20000              /* must be >= 2*n for the required n (≈10000) */

/* outside the function we can only use types seq_t, idx_t as supplied by the build */

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t terms[LIMIT/2];      /* at most LIMIT/2 even numbers */
    static size_t terms_len = 0;

    if (!init) {
        /* 1.  Sieve of Eratosthenes up to LIMIT+2 (for isolated test) */
        static bool is_prime[LIMIT + 3];
        for (idx_t i = 0; i <= LIMIT + 2; ++i)
            is_prime[i] = false;
        for (idx_t i = 2; i <= LIMIT + 2; ++i)
            is_prime[i] = true;
        for (idx_t i = 2; i * i <= LIMIT + 2; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= LIMIT + 2; j += i)
                    is_prime[j] = false;
            }
        }

        /* 2.  Collect all primes up to LIMIT */
        static uint64_t primes[3000];
        size_t primes_cnt = 0;
        for (idx_t i = 2; i <= LIMIT; ++i)
            if (is_prime[i])
                primes[primes_cnt++] = (uint64_t)i;

        /* 3.  Identify isolated primes */
        static uint64_t isolated[3000];
        size_t isolated_cnt = 0;
        for (size_t k = 0; k < primes_cnt; ++k) {
            uint64_t p = primes[k];
            bool left  = (p >= 3) ? is_prime[(idx_t)(p - 2)] : false;
            bool right = (p + 2 <= LIMIT + 2) ? is_prime[(idx_t)(p + 2)] : false;
            if (!left && !right)
                isolated[isolated_cnt++] = p;
        }

        /* 4.  Mark all even sums of two isolated primes */
        static bool rep[LIMIT + 3];
        for (idx_t i = 0; i <= LIMIT; ++i)
            rep[i] = false;
        for (size_t i = 0; i < isolated_cnt; ++i) {
            uint64_t p = isolated[i];
            for (size_t j = i; j < isolated_cnt; ++j) {
                uint64_t q = isolated[j];
                uint64_t s = p + q;
                if (s > (uint64_t)LIMIT) break;
                rep[(size_t)s] = true;
            }
        }

        /* 5.  Store all even numbers that are not representable */
        for (idx_t even = 2; even <= LIMIT; even += 2) {
            if (!rep[(size_t)even]) {
                if (terms_len < LIMIT/2)
                    terms[terms_len++] = (seq_t)even;
            }
        }

        init = true;
    }

    if (n < 1 || (idx_t)(n - 1) >= terms_len)
        return (seq_t)0;
    return terms[(size_t)(n - 1)];
}
