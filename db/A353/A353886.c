/*
 * A353886  Nonnegative numbers k such that k^2 + k + 1 is squarefree.
 * https://oeis.org/A353886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A353886:  numbers k such that k^2 + k + 1 is square‑free
 *
 *  a(n) = k on the n‑th place where k^2 + k + 1 contains no
 *  squared prime factor.  a(1) = 0.
 *
 *  Implementation uses a lightweight prime sieve (up to 200 000) and
 *  tests square‑freeness by checking that no prime squared divides
 *  k^2 + k + 1.
 *
 *  The sequence is generated on demand; no external libraries are used.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------
 *  Prime helper
 * ------------------------------------------------------------ */
#define PRIME_LIMIT 200000
#define MAX_PRIMES  200000

static uint32_t _primes[MAX_PRIMES];
static size_t   _primes_cnt  = 0;
static bool     _primes_init = false;

/* Simple sieve up to PRIME_LIMIT */
static void init_primes(void)
{
    if (_primes_init) return;
    bool *sieve = (bool *)malloc(PRIME_LIMIT + 1);
    if (!sieve) return;                     /* fatal error → return nothing useful */
    memset(sieve, 0, PRIME_LIMIT + 1);

    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 1;
        }
    }
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            _primes[_primes_cnt++] = i;
        }
    }
    free(sieve);
    _primes_init = true;
}

/* ------------------------------------------------------------
 *  Square‑free test for a 128‑bit integer
 * ------------------------------------------------------------ */
static bool squarefree_128(unsigned __int128 m)
{
    for (size_t i = 0; i < _primes_cnt; ++i) {
        unsigned __int128 p  = _primes[i];
        unsigned __int128 pp = p * p;
        if (pp > m) break;
        if ((m % pp) == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------
 *  Public sequence function
 * ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* no defined value for offset 0 */
    init_primes();                        /* ensure primes are ready */

    idx_t k      = 0;                     /* current integer to test */
    idx_t count  = 0;                     /* how many valid k found */

    while (true) {
        unsigned __int128 m = (unsigned __int128)k * k + k + 1;
        if (squarefree_128(m)) {
            ++count;
            if (count == n)
                return (seq_t)k;          /* k fits in seq_t if it does in the build */
        }
        ++k;
    }
}
