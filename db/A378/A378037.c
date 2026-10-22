/*
 * A378037  First differences of A112926 (smallest squarefree integer >
 *          prime(n)).
 * https://oeis.org/A378037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = S(n+1) – S(n) where S(k) is the smallest square‑free
 *            integer > prime(k)
 * Range:     u8 n <= 54, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   is_prime, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"          /* bool is_prime(idx_t x) */
#include "../utils/is_squarefree.h"     /* bool is_squarefree(seq_t x) */

seq_t a(idx_t n)
{
    /* 1‑indexed sequence:  a(1) corresponds to the first listed term. */

    /* -------------  Memoised data for S(k) ------------- */
    static seq_t *S      = NULL;           /* array of S(k)  (S[0] = S(1))   */
    static size_t S_cap  = 0;              /* capacity of S           */
    static size_t S_len  = 0;              /* number of stored S(k)   */

    /* -------------  Memoised prime list ------------- */
    static uint32_t *primes   = NULL;      /* array of primes         */
    static size_t primes_cap = 0;          /* capacity                 */
    static size_t prime_cnt  = 0;          /* number of primes stored */
    static idx_t   next_candidate = 2;     /* next number to test     */

    /* Ensure we know S(n+1) and S(n).  n>=1 in the OEIS catalogue; */
    /* behaviour for n=0 is irrelevant but we guard anyway. */
    if (n == 0) return 0;

    /* We need values for k = 1 … n+1  (S[0] … S[n]) */
    size_t required = (size_t)n + 1;
    if (S_len < required) {
        /* Grow S array if needed */
        if (required > S_cap) {
            size_t new_cap = S_cap ? S_cap * 2 : required;
            while (new_cap < required) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(S, new_cap * sizeof(seq_t));
            if (!tmp) return 0;   /* allocation failure – safe return */
            S = tmp;
            S_cap = new_cap;
        }

        /* ---  Generate new primes needed  --- */
        if (prime_cnt < required) {
            if (prime_cnt == 0) {
                /* Ensure primes[0] = 2 */
                if (primes_cap == 0) {
                    primes_cap = 16;
                    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
                    if (!primes) return 0;
                }
                primes[prime_cnt++] = 2;
                if (next_candidate == 2) next_candidate = 3;
            }
            while (prime_cnt < required) {
                if (is_prime(next_candidate)) {
                    /* Grow primes array if needed */
                    if (prime_cnt == primes_cap) {
                        size_t newp = primes_cap ? primes_cap * 2 : 16;
                        uint32_t *tmpP = (uint32_t *)realloc(primes,
                                                             newp * sizeof(uint32_t));
                        if (!tmpP) return 0;
                        primes = tmpP;
                        primes_cap = newp;
                    }
                    primes[prime_cnt++] = (uint32_t)next_candidate;
                }
                ++next_candidate;
            }
        }

        /* ---  Compute S(k) for new k's  --- */
        for (size_t k = S_len + 1; k <= required; ++k) {
            uint32_t p = primes[k - 1];          /* k‑th prime (S[0] = first prime) */
            uint64_t m = (uint64_t)p + 1;        /* candidate > prime(k) */
            while (!is_squarefree((seq_t)m))
                ++m;
            S[k - 1] = (seq_t)m;
        }
        S_len = required;
    }

    /* a(n) = S(n+1) – S(n)  →  S[n] – S[n‑1]  */
    return S[(size_t)n] - S[(size_t)n - 1];
}
