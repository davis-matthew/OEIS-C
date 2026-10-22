/*
 * A380333  Where prime race 8m+3 vs. 8m+7 is tied.
 * https://oeis.org/A380333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A380333: the n-th prime p such that the number of primes ≤p
   congruent to 3 (mod 8) equals the number congruent to 7 (mod 8).
   The function searches primes in ascending order, keeps the
   running difference D = pi_{8,3}(p) – pi_{8,7}(p) and stores every
   prime for which D becomes zero.  The n‑th stored prime is returned. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- helper to grow a dynamic array ---------------------------------- */
static void grow_if_needed(void **ptr, size_t *cap, size_t need, size_t elem_size)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < need) new_cap <<= 1;
    *ptr = realloc(*ptr, new_cap * elem_size);
    *cap = new_cap;
}

/* --- primality test using a list of previously found primes ------------- */
static bool is_prime(uint64_t n, const uint64_t *primes, size_t prime_cnt)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* --- sequence value --------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static state retained between calls */
    static uint64_t *prime_list = NULL;     /* list of all discovered primes   */
    static size_t prime_cap   = 0;
    static size_t prime_len   = 0;

    static seq_t *tie_cache = NULL;        /* primes where the race is tied */
    static size_t tie_cap   = 0;
    static size_t tie_len   = 0;

    static int diff = 0;                    /* pi3 - pi7                          */
    static uint64_t current = 2;            /* next integer to test for primality */

    /* generate primes and record ties until we have at least n ties */
    while (tie_len < n) {
        /* test if current is prime */
        if (is_prime(current, prime_list, prime_len)) {
            /* store new prime */
            grow_if_needed((void**)&prime_list, &prime_cap,
                           prime_len + 1, sizeof(uint64_t));
            prime_list[prime_len++] = current;

            /* update the race count for residues 3 and 7 mod 8 */
            switch (current & 7) {           /* current % 8 */
                case 3: ++diff; break;
                case 7: --diff; break;
                default: break;               /* residues 1 and 5 do nothing */
            }

            /* a tie occurs when the difference becomes zero */
            if (diff == 0) {
                grow_if_needed((void**)&tie_cache, &tie_cap,
                               tie_len + 1, sizeof(seq_t));
                tie_cache[tie_len++] = (seq_t)current;
            }
        }
        ++current;
    }

    return tie_cache[n - 1];
}
